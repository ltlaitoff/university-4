Object.typedKeys = Object.keys

/**
 * @type {Record<UIVariants, string>}
 */
const UIVariants = Object.freeze({
	LSB_ENCODE: 'LSB_ENCODE',
	LSB_DECODE: 'LSB_DECODE',
	BPCS_ENCODE: 'BPCS_ENCODE',
	BPCS_DECODE: 'BPCS_DECODE'
})

/**
 * @type LSBKeyParams[]
 */
const LSB_KEY_PARAMS = [
	'StartX',
	'StartY',
	'EndX',
	'EndY',
	'GapX',
	'GapY',
	'ChannelsPerPixel',
	'Channels'
]
/**
 * @type Record<LSBKeyParams, string>
 */
const LSB_KEY_PARSING_SCHEMA = {
	StartX: 'S',
	StartY: 'T',
	EndX: 'E',
	EndY: 'N',
	GapX: 'H',
	GapY: 'V',
	ChannelsPerPixel: 'P',
	Channels: 'C'
}

/**
 * @type {Config}
 */
const config = {
	wasmUrl: './main.wasm',

	globalIds: {
		originalImageInput: { id: 'original-input', type: HTMLInputElement },
		originalImageInputDropZone: {
			id: 'original-input-drop-zone',
			type: HTMLLabelElement
		},
		originalImagePreview: { id: 'original-preview', type: HTMLImageElement },
		resultImagePreview: { id: 'result-preview', type: HTMLImageElement },
		submitButton: { id: 'submit-button', type: HTMLButtonElement }
	},

	menu: {
		name: {
			methods: 'menu-method',
			operation: 'menu-operation'
		},
		value: {
			methods: ['lsb', 'bpcs'],
			operation: ['encode', 'decode']
		}
	},

	// prettier-ignore
	UIids: {
		menu: {
			base: { id: "menu", type: HTMLDivElement },
			methods: { id: "menu-methods", type: HTMLDivElement },
		},
		lsbBlock: { id: "lsb", type: HTMLDivElement },
		encodeBlock: { id: "encode", type: HTMLDivElement },
		decodeBlock: { id: "decode", type: HTMLDivElement },
		swapButton: { id: "swap", type: HTMLButtonElement },

		ERROR: {
			block: { id: "error", type: HTMLDivElement },
			message: { id: "error-message", type: HTMLDivElement },
			button: { id: "error-close", type: HTMLButtonElement },
		}
	},

	// prettier-ignore
	ids: {
		DEBUG: { id: 'debug', type: HTMLInputElement },

		SHARED: {
			secretAsFileCheckbox: { id: 'secret-as-file', type: HTMLInputElement }
		},
		ENCODE: {
			secretMessageInput: { id: 'encode-secret-message-input', type: HTMLInputElement },
			secretFileInput: { id: 'encode-secret-file-input', type: HTMLInputElement },
			secretMessageInputBlock: { id: 'encode-secret-message-input-block', type: HTMLLabelElement },
			secretFileInputBlock: { id: 'encode-secret-file-input-block', type: HTMLLabelElement },
		},
		DECODE: {
			secretMessageOutputBlock: { id: 'decode-secret-message-output-block', type: HTMLLabelElement },
			secretFileOutputButtonBlock: { id: 'decode-secret-file-output-button-block', type: HTMLDivElement },
			secretMessageOutput: { id: 'decode-secret-message-output', type: HTMLInputElement },
			secretFileOutputButton: { id: 'decode-secret-file-output-button', type: HTMLButtonElement }
		},
		LSB: {
			keyInputBlock: { id: "lsb-secret-key-block", type: HTMLDivElement },
			keyInputOutput: { id: 'lsb-secret-key', type: HTMLInputElement },
		},
	}
}

// prettier-ignore
const GLOBAL = {
	originalImagePreview: loadElement(config.globalIds.originalImagePreview),
	originalImageInputDropZone: loadElement(config.globalIds.originalImageInputDropZone),
	resultImagePreview: loadElement(config.globalIds.resultImagePreview),
	submitButton: loadElement(config.globalIds.submitButton),
	originalImageInput: loadElement(config.globalIds.originalImageInput),
}

const UI = {
	menu: {
		base: loadElement(config.UIids.menu.base),
		methods: loadElement(config.UIids.menu.methods)
	},
	lsbBlock: loadElement(config.UIids.lsbBlock),
	encodeBlock: loadElement(config.UIids.encodeBlock),
	decodeBlock: loadElement(config.UIids.decodeBlock),
	swapButton: loadElement(config.UIids.swapButton),
	ERROR: {
		block: loadElement(config.UIids.ERROR.block),
		message: loadElement(config.UIids.ERROR.message),
		button: loadElement(config.UIids.ERROR.button)
	}
}

const SHARED = {
	secretAsFileCheckbox: loadElement(config.ids.SHARED.secretAsFileCheckbox),
	encode: {
		secretMessageInput: loadElement(config.ids.ENCODE.secretMessageInput),
		secretFileInput: loadElement(config.ids.ENCODE.secretFileInput),
		secretMessageInputBlock: loadElement(
			config.ids.ENCODE.secretMessageInputBlock
		),
		secretFileInputBlock: loadElement(config.ids.ENCODE.secretFileInputBlock)
	},
	decode: {
		secretMessageOutput: loadElement(config.ids.DECODE.secretMessageOutput),
		secretFileOutputButton: loadElement(
			config.ids.DECODE.secretFileOutputButton
		),
		secretMessageOutputBlock: loadElement(
			config.ids.DECODE.secretMessageOutputBlock
		),
		secretFileOutputButtonBlock: loadElement(
			config.ids.DECODE.secretFileOutputButtonBlock
		)
	}
}

// prettier-ignore
const LSB = {
	keyInputBlock: loadElement(config.ids.LSB.keyInputBlock),
	keyInputOutput: loadElement(config.ids.LSB.keyInputOutput),
}

const DEBUG = loadElement(config.ids.DEBUG)

/**
 * @type {State}
 */
const state = {
	activeMethod: 'LSB',
	activeOperation: 'ENCODE',
	debugMode: false,

	originalImageFile: undefined,
	resultImageFile: undefined,
	secretAsFile: false,
	secretMessage: '',
	encodeSecretFile: undefined,
	decodedSecretFile: undefined,

	LSB: {
		key: {
			StartX: 0,
			StartY: 0,
			EndX: 0,
			EndY: 0,
			GapX: 0,
			GapY: 0,
			ChannelsPerPixel: 3,
			Channels: ['R', 'G', 'B']
		}
	},

	errorMessage: ''
}

// --

window.addEventListener('drop', e => {
	if (
		e.dataTransfer &&
		Array.from(e.dataTransfer.items).some(item => item.kind === 'file')
	) {
		e.preventDefault()
	}
})

window.addEventListener('dragover', e => {
	if (!e.target) return

	const fileItems =
		e.dataTransfer &&
		Array.from(e.dataTransfer.items).filter(item => item.kind === 'file')

	if (fileItems && fileItems.length > 0) {
		e.preventDefault()

		assert(e.target instanceof Node, 'e.target should be Node')

		if (!GLOBAL.originalImageInputDropZone.contains(e.target)) {
			e.dataTransfer.dropEffect = 'none'
		}
	}
})

GLOBAL.originalImageInputDropZone.addEventListener('dragover', e => {
	const fileItems =
		e.dataTransfer &&
		Array.from(e.dataTransfer.items).filter(item => item.kind === 'file')

	if (fileItems && fileItems.length > 0) {
		e.preventDefault()

		if (fileItems.some(item => item.type.startsWith('image/'))) {
			e.dataTransfer.dropEffect = 'copy'
		} else {
			e.dataTransfer.dropEffect = 'none'
		}
	}
})

GLOBAL.originalImageInputDropZone.addEventListener('drop', e => {
	e.preventDefault()

	const files =
		e.dataTransfer &&
		Array.from(e.dataTransfer.items)
			.map(item => item.getAsFile())
			.filter(file => file)

	const firstFile = files?.[0]

	if (!firstFile) {
		showError('No file selected. Please choose a file.')
	}

	state.originalImageFile = firstFile
	render()
})

UI.ERROR.button.addEventListener('click', e => {
	assert(
		e.target instanceof config.UIids.ERROR.button.type,
		'Event target on close error button should be equal to config type!'
	)

	state.errorMessage = ''
	render()
})

LSB.keyInputBlock.addEventListener('change', e => {
	assert(
		e.target instanceof HTMLInputElement,
		'Key input block change should be called only on input element!'
	)

	if (e.target.name == 'key') {
		if (e.target.value == '') return

		let result

		try {
			result = goParseLSBKey(e.target.value)
		} catch (err) {
			errorHandler(err)
			return
		}

		assert(
			result !== undefined,
			'Golang function result should be always defined'
		)

		if (result.ok == false) {
			showError(`Error! ${result.message}`)
			return
		}

		const parsedKey = result.data
		console.log(parsedKey)
		state.LSB.key = parsedKey

		render()
		return
	}

	assert(
		LSB_KEY_PARAMS.includes(e.target.name),
		'Key input block input name should be one of LSB key params'
	)

	if (e.target.name === 'Channels') {
		const value = e.target.value.split('')
		state.LSB.key[e.target.name] = value

		render()
		return
	}

	const value = Number(e.target.value)
	assert(Number.isNaN(value) === false, 'Input number value should not be NaN!')

	state.LSB.key[e.target.name] = value
	render()
})

SHARED.decode.secretFileOutputButton.addEventListener('click', e => {
	assert(
		e.target instanceof config.ids.DECODE.secretFileOutputButton.type,
		'Event target of lsb decode file output button should be equalt to config value'
	)

	assert(
		state.decodedSecretFile !== undefined,
		'Decoded secret file lsb should be defined on click on button'
	)

	const url = URL.createObjectURL(state.decodedSecretFile)

	const a = document.createElement('a')
	a.href = url
	a.download = state.decodedSecretFile.name
	document.body.appendChild(a)
	a.click()

	a.remove()
	URL.revokeObjectURL(url)
})

SHARED.encode.secretFileInput.addEventListener('change', e => {
	assert(
		e.target instanceof config.ids.ENCODE.secretFileInput.type,
		'Event target of lsb secret file input should be equal to config file'
	)
	assert(e.target.files !== null, 'LSB secret file input should have files')

	const file = e.target.files[0]
	if (!file) {
		showError('No file selected. Please choose a file.')
	}

	state.encodeSecretFile = file
})

SHARED.secretAsFileCheckbox.addEventListener('change', e => {
	assert(
		e.target instanceof config.ids.SHARED.secretAsFileCheckbox.type,
		'Event target on click lsb secret as file should be same type as config'
	)

	state.secretAsFile = e.target.checked

	render()
})

UI.swapButton.addEventListener('click', e => {
	assert(
		e.target instanceof config.UIids.swapButton.type,
		'Event target on click swap should be same type as config swap button'
	)

	if (state.originalImageFile === undefined) return
	if (state.resultImageFile === undefined) return

	const temp = state.originalImageFile
	state.originalImageFile = state.resultImageFile
	state.resultImageFile = temp

	render()
})

DEBUG.addEventListener('change', e => {
	assert(
		e.target instanceof config.ids.DEBUG.type,
		'Event target on change debug should be same type as debug'
	)

	state.debugMode = e.target.checked
	goDebug(state.debugMode)
})

GLOBAL.originalImageInput.addEventListener('change', e => {
	assert(
		e.target instanceof config.globalIds.originalImageInput.type,
		'Event target on change image input should be same type as type from config'
	)
	assert(e.target.files !== null, 'Original image input should have files')

	const file = e.target.files[0]
	if (!file) {
		showError('No file selected. Please choose a file.')
	}

	state.originalImageFile = file
	render()
})

SHARED.encode.secretMessageInput.addEventListener('change', e => {
	assert(
		e.target !== null,
		'Secret message input on change event target is null!'
	)
	assert(
		e.target instanceof config.ids.ENCODE.secretMessageInput.type,
		'Secret message input on change event target should have the same type from config!'
	)

	state.secretMessage = e.target.value
})

async function prepareSecretMessage() {
	if (state.secretAsFile === true) {
		if (state.encodeSecretFile === undefined) {
			showError('Secret file should exists to run LSB encoding')
		}

		return await fileToByteArray(state.encodeSecretFile)
	}

	if (state.secretMessage === '') {
		showError('Secret message is empty!')
	}

	return new TextEncoder().encode(state.secretMessage)
}

async function submitLSBEncode() {
	if (state.originalImageFile === undefined) {
		showError('Image is not loaded!')
	}

	const message = await prepareSecretMessage()
	const originalImage = await fileToByteArray(state.originalImageFile)

	let result = undefined
	try {
		result = goEncodeLSB(
			originalImage,
			state.originalImageFile.type,
			message,
			generateLsbKey()
		)
	} catch (err) {
		errorHandler(err)
		return
	}

	assert(
		result !== undefined,
		'Golang function result should be always defined'
	)

	if (result.ok == false) {
		showError(`Error! ${result.message}`)
		return
	}

	const content = result.data
	console.log('JS result:', content, typeof content)

	let resultImageType = state.originalImageFile.type

	if (state.originalImageFile.type === 'image/jpeg') {
		resultImageType = 'image/png'
	}

	const blob = new Blob([content], { type: resultImageType })
	state.resultImageFile = new File([blob], `result.${blob.type}`, {
		type: blob.type
	})

	render()
}

async function submitLSBDecode() {
	if (state.originalImageFile === undefined) {
		showError('Image is not loaded!')
	}

	const originalImage = await fileToByteArray(state.originalImageFile)

	let decodeImageType = state.originalImageFile.type

	if (state.originalImageFile.type === 'image/jpeg') {
		decodeImageType = 'image/png'
	}

	let result

	try {
		result = goDecodeLSB(originalImage, decodeImageType, generateLsbKey())
	} catch (err) {
		errorHandler(err)
		return
	}

	assert(
		result !== undefined,
		'Golang function result should be always defined'
	)

	if (result.ok == false) {
		showError(`Error! ${result.message}`)
		return
	}

	const content = result.data

	if (state.secretAsFile) {
		state.decodedSecretFile = new File([content], `result`)
	} else {
		SHARED.decode.secretMessageOutput.value = content.toString()
	}

	console.log('Decoded message js:', content)

	render()
}

async function submitBPCSEncode() {
	if (state.originalImageFile === undefined) {
		showError('Image is not loaded!')
	}

	const message = await prepareSecretMessage()
	const originalImage = await fileToByteArray(state.originalImageFile)

	let result
	try {
		result = goEncodeBPCS(originalImage, state.originalImageFile.type, message)
	} catch (err) {
		errorHandler(err)
		return
	}

	assert(
		result !== undefined,
		'Golang function result should be always defined'
	)

	if (result.ok == false) {
		showError(`Error! ${result.message}`)
		return
	}

	const content = result.data

	let resultImageType = state.originalImageFile.type
	if (state.originalImageFile.type === 'image/jpeg') {
		resultImageType = 'image/png'
	}

	const blob = new Blob([content], { type: resultImageType })
	state.resultImageFile = new File([blob], `result.${blob.type}`, {
		type: blob.type
	})

	render()
}

async function submitBPCSDecode() {
	if (state.originalImageFile === undefined) {
		showError('Image is not loaded!')
	}

	const originalImage = await fileToByteArray(state.originalImageFile)

	let decodeImageType = state.originalImageFile.type
	if (state.originalImageFile.type === 'image/jpeg') {
		decodeImageType = 'image/png'
	}

	let result
	try {
		result = goDecodeBPCS(originalImage, decodeImageType)
	} catch (err) {
		errorHandler(err)
		return
	}

	assert(
		result !== undefined,
		'Golang function result should be always defined'
	)

	if (result.ok == false) {
		showError(`Error! ${result.message}`)
		return
	}

	const content = result.data

	if (state.secretAsFile) {
		state.decodedSecretFile = new File([content], `result`)
	} else {
		SHARED.decode.secretMessageOutput.value = content.toString()
	}

	console.log('[JS] BPCS decoded message:', content)

	render()
}

GLOBAL.submitButton.addEventListener('click', () => {
	state.errorMessage = ''
	render()

	if (state.activeMethod == 'LSB') {
		if (state.activeOperation == 'ENCODE') {
			submitLSBEncode()
		}

		if (state.activeOperation == 'DECODE') {
			submitLSBDecode()
		}
	}

	if (state.activeMethod == 'BPCS') {
		if (state.activeOperation == 'ENCODE') {
			submitBPCSEncode()
		}

		if (state.activeOperation == 'DECODE') {
			submitBPCSDecode()
		}
	}
})

UI.menu.base.addEventListener('change', e => {
	assert(
		e.target instanceof HTMLInputElement,
		'Change event in menu should be only on HTMLInputElement'
	)

	assert(
		e.target.name == config.menu.name.methods ||
			e.target.name == config.menu.name.operation,
		'Menu input name should be one of menu names'
	)

	assert(
		config.menu.value.methods.includes(e.target.value) ||
			config.menu.value.operation.includes(e.target.value),
		'Menu input value should be one from menu config'
	)

	if (e.target.name == config.menu.name.methods) {
		if (e.target.checked) {
			if (e.target.value == 'lsb') {
				state.activeMethod = 'LSB'
			}

			if (e.target.value == 'bpcs') {
				state.activeMethod = 'BPCS'
			}
		}
	}

	if (e.target.name == config.menu.name.operation) {
		if (e.target.checked) {
			if (e.target.value == 'encode') {
				state.activeOperation = 'ENCODE'
			}

			if (e.target.value == 'decode') {
				state.activeOperation = 'DECODE'
			}
		}
	}

	render()
})

async function main() {
	render()

	const go = new Go()

	const wasmModule = await WebAssembly.instantiateStreaming(
		fetch(config.wasmUrl),
		go.importObject
	)

	go.run(wasmModule.instance)
}

main()

function render() {
	if (state.activeOperation === 'ENCODE') {
		UI.encodeBlock.classList.remove('hidden')
		UI.decodeBlock.classList.add('hidden')
	} else {
		UI.encodeBlock.classList.add('hidden')
		UI.decodeBlock.classList.remove('hidden')
	}

	if (state.activeMethod == 'LSB') {
		UI.lsbBlock.classList.remove('hidden')
	} else if (state.activeMethod == 'BPCS') {
		UI.lsbBlock.classList.add('hidden')
	}

	if (state.originalImageFile) {
		GLOBAL.originalImagePreview.src = URL.createObjectURL(
			state.originalImageFile
		)
		GLOBAL.originalImagePreview.classList.remove('hidden')
		GLOBAL.originalImageInputDropZone.classList.add('hidden')
	} else {
		GLOBAL.originalImagePreview.classList.add('hidden')
		GLOBAL.originalImageInputDropZone.classList.remove('hidden')
	}

	if (state.resultImageFile) {
		GLOBAL.resultImagePreview.src = URL.createObjectURL(state.resultImageFile)
	}

	SHARED.secretAsFileCheckbox.checked = state.secretAsFile

	if (state.secretAsFile) {
		SHARED.encode.secretMessageInputBlock.classList.add('hidden')
		SHARED.decode.secretMessageOutputBlock.classList.add('hidden')
		SHARED.encode.secretFileInputBlock.classList.remove('hidden')
		SHARED.decode.secretFileOutputButtonBlock.classList.remove('hidden')
	} else {
		SHARED.encode.secretMessageInputBlock.classList.remove('hidden')
		SHARED.decode.secretMessageOutputBlock.classList.remove('hidden')
		SHARED.encode.secretFileInputBlock.classList.add('hidden')
		SHARED.decode.secretFileOutputButtonBlock.classList.add('hidden')
	}

	if (state.decodedSecretFile) {
		SHARED.decode.secretFileOutputButton.disabled = false
	} else {
		SHARED.decode.secretFileOutputButton.disabled = true
	}

	if (state.errorMessage !== '') {
		UI.ERROR.block.dataset['active'] = 'true'
		UI.ERROR.message.textContent = state.errorMessage
	} else {
		UI.ERROR.block.dataset['active'] = 'false'
		UI.ERROR.message.textContent = state.errorMessage
	}

	LSB.keyInputOutput.value = generateLsbKey()
	setLSBKeyFields()
}

function setLSBKeyFields() {
	for (const key of LSB_KEY_PARAMS) {
		const input = LSB.keyInputBlock.querySelector(`[name=${key}]`)
		assert(
			input instanceof HTMLInputElement,
			`LSB key field with name ${key} should be HTMLInputElement`
		)

		if (key == 'Channels') {
			input.value = state.LSB.key[key].join('')
			continue
		}

		input.value = String(state.LSB.key[key])
	}
}

function generateLsbKey() {
	let result = ''

	for (const key of LSB_KEY_PARAMS) {
		if (state.LSB.key[key] !== undefined) {
			if (key !== 'Channels') {
				result += LSB_KEY_PARSING_SCHEMA[key] + state.LSB.key[key]
			} else {
				result += state.LSB.key[key]
					.map(el => LSB_KEY_PARSING_SCHEMA[key] + el)
					.join('')
			}
		}
	}

	return result
}

/**
 * @type {ErrorHandler}
 */
function errorHandler(err) {
	if (err instanceof WebAssembly.RuntimeError) {
		showError('Catch WebAssembly.RuntimeError! Check console for more details!')
		return
	}

	console.log('Error', err)
	showError('Catch unknown error! Check console for more details!')
}

/**
 * @type {ShowError}
 */
function showError(message) {
	console.log(message)
	state.errorMessage = message
	render()
}

/**
 * @type {FileToByteArray}
 */
async function fileToByteArray(file) {
	return file.arrayBuffer().then(value => new Uint8Array(value))
}

/**
 * @type {LoadElement}
 */
function loadElement(elementInfo) {
	const element = document.querySelector('#' + elementInfo.id)
	assert(
		element !== null,
		`Element with id \"#${elementInfo.id}\" should not be null!`
	)
	assert(
		element instanceof elementInfo.type,
		`Element with id \"#${elementInfo.id}\" should be instance of ${elementInfo.type.name}!`
	)

	return element
}

/**
 * @type {Assert}
 */
function assert(condition, message) {
	if (!condition) {
		showError('[ASSERTION]: ' + message)
	}
}

/**
 * @type {IsRecord}
 */
function isRecord(value) {
	return typeof value === 'object' && value !== null && !Array.isArray(value)
}
