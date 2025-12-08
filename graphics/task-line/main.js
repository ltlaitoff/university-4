const ROOT = document.querySelector("#root")
const p1Element = document.querySelector("#p1")
const p2Element = document.querySelector("#p2")

// Number of elements in row and col, all matrix is N*N
const N = 15

// Size of one cell, in px
const CELL = 50

document.documentElement.style.setProperty("--N", N);
document.documentElement.style.setProperty("--cell", `${CELL}px`);

let isFour = false

// map[int - Y]map[int - X]bool
let activeCells = { }

const P1 = { x: 2, y: 4 }
const P2 = { x: 11, y: 7 }
const points = { first: P1, second: P2 }

function isCellActive(x, y) {
	const yData = activeCells[y]
	if (!yData) return false

	const xData = yData[x]
	if (!xData) return false

	return true
}

function createPoint(x, y, order) {
	const point = document.createElement("div")
	point.classList.add("point")

	point.style.setProperty("--x", x);
	point.style.setProperty("--y", y);

	if (order == "first") {
		point.classList.add("point--fisrt")
	}

	if (order == "second") {
		point.classList.add("point--second")
	}

	return point
}

function render(points) {
	const grid = document.createElement("div")
	grid.className = "field"

	for (let row = 0; row < N; row++) {
		for (let col = 0; col < N; col++) {
			const cell = document.createElement("div")
			cell.className = "field-cell"

			if (isCellActive(col, row)) {
				cell.classList.add("field-cell--active")
			}

			cell.dataset.x = col
			cell.dataset.y = row

			grid.append(cell)
		}
	}

	grid.appendChild(createPoint(points.first.x, points.first.y, "first"))
	grid.appendChild(createPoint(points.second.x, points.second.y, "second"))

	p1Element.textContent = `P1: (${P1.x}, ${P1.y})`
	p2Element.textContent = `P2: (${P2.x}, ${P2.y})`
	ROOT.replaceChildren(grid)
}

function setPoint(x, y) {
	if (activeCells[y] == undefined) {
		activeCells[y] = {}
	}

	activeCells[y][x] = true
}

// Based on https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm 
function line(x0, y0, x1, y1) {
	const dx = Math.abs(x1 - x0)
	const dy = -Math.abs(y1 - y0)

	const sx = x0 < x1 ? 1 : -1
	const sy = y0 < y1 ? 1 : -1

	let error = dx + dy

	while (true) {
		setPoint(x0, y0)
		const e2 = 2 * error

		if (x0 === x1 && y0 === y1) break

		if (!isFour) {
			if (e2 >= dy) {
				if (x0 == x1) break

				error += dy
				x0 += sx
			}

			if (e2 <= dx) {
				if (y0 == y1) break

				error += dx
				y0 += sy
			}
		}

		if (isFour) {
			if (e2 <= dx) {
				if (y0 == y1) break

				error += dx
				y0 += sy
			} else if (e2 >= dy) {
				if (x0 == x1) break

				error += dy
				x0 += sx
			}
		}
	}
}

document.querySelector("#four").addEventListener("click", e => {
	const target = e.target
	isFour = target.checked
	main()
})

document.addEventListener("click", e => {
	const target = e.target

	if (target.classList.contains("field-cell")) {
		P1.x = Number(target.dataset.x)
		P1.y = Number(target.dataset.y)

		main()
	}
})

document.addEventListener("contextmenu", e => {
	e.preventDefault()
	const target = e.target

	if (target.classList.contains("field-cell")) {
		P2.x = Number(target.dataset.x)
		P2.y = Number(target.dataset.y)
		main()
	}
})

function main() {
	console.log("Main!")

	activeCells = {}
	line(P1.x, P1.y, P2.x, P2.y)
	render(points)
}

main()
