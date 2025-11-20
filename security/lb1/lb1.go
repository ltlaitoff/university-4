package lb1

import (
	"errors"
	"fmt"
	"regexp"
	"strings"
)

const WITH_PUNCTIATION_AND_NUMBERS = false

const WIDTH = 5
const HEIGHT = 5

type polybiusSquare struct {
	square  [][]rune
	charMap map[rune][2]int
}

// Base preparation, like remove punctuation and change all letters to uppercase
func prepareData(text string) string {
	if WITH_PUNCTIATION_AND_NUMBERS {
		return strings.ToUpper(text)
	}

	removePunctiation := regexp.MustCompile(`([^\w])|([\s\d])`)
	withoutPunctiation := removePunctiation.ReplaceAllLiteralString(text, "")

	return strings.ToUpper(withoutPunctiation)
}

func (square polybiusSquare) print() {
	fmt.Println("Polybius square:")

	for i, _ := range square.square {
		for j, _ := range square.square[i] {
			value := square.square[i][j]

			if value == 0 {
				value = '-'
			}

			fmt.Print(string(value) + " ")
		}

		fmt.Print("\n")
	}
}

func isCodeValid(code string) bool {
	cache := make(map[rune]bool)

	for _, r := range code {
		_, ok := cache[r]

		if ok {
			return false
		}

		cache[r] = true
	}

	return true
}

// Generate square for latin alphabet
// Default size if 5x5, where I should be equal to J
func (polybiusSquare *polybiusSquare) init(code string) {
	square := make([][]rune, HEIGHT)

	for i := range square {
		square[i] = make([]rune, WIDTH)
	}

	charMap := make(map[rune][2]int)
	polybiusSquare.square = square
	polybiusSquare.charMap = charMap

	cache := make(map[rune]bool)
	cache['J'] = true

	for row := range HEIGHT {
		for col := range WIDTH {
			index := row*WIDTH + col

			if index >= len(code) {
				break
			}

			char := rune(code[index])

			if char == 'J' {
				char = 'I'
			}

			square[row][col] = char
			charMap[char] = [2]int{row, col}
			cache[char] = true
		}
	}

	queue := make([]rune, 0)

	for char := 'A'; char <= 'Z'; char++ {
		if _, ok := cache[char]; ok {
			continue
		}

		queue = append(queue, char)
	}

	for row := range HEIGHT {
		for col := range WIDTH {
			if square[row][col] != 0 {
				continue
			}

			char := queue[0]
			queue = queue[1:]
			square[row][col] = char
			charMap[char] = [2]int{row, col}
		}
	}
}

func (square polybiusSquare) encode(char rune) (string, error) {
	pos, ok := square.charMap[char]

	if !ok {
		if !WITH_PUNCTIATION_AND_NUMBERS {
			return "", errors.New("Char not found! Char: " + string(char))
		}

		return string(char), nil
	}

	newRow := pos[0] + 1

	if newRow > HEIGHT-1 {
		newRow = 0
	}

	return string(square.square[newRow][pos[1]]), nil
}

func (square polybiusSquare) decode(char rune) (string, error) {
	pos, ok := square.charMap[char]

	if !ok {
		if !WITH_PUNCTIATION_AND_NUMBERS {
			return "", errors.New("Char not found! Char: " + string(char))
		}

		return string(char), nil
	}

	newRow := pos[0] - 1

	if newRow < 0 {
		newRow = HEIGHT - 1
	}

	return string(square.square[newRow][pos[1]]), nil
}

func encodePolybiusSquare(sourceData string, sourceCode string) (string, error) {
	if !isCodeValid(sourceCode) {
		return "", errors.New("Code is not valid")
	}

	data := prepareData(sourceData)
	code := prepareData(sourceCode)
	fmt.Println("[EN] Prepared text:", data)
	fmt.Println("[EN] Prepared code:", code)

	square := polybiusSquare{}
	square.init(code)
	square.print()

	encodedData := ""

	for _, char := range data {
		encodedChar, err := square.encode(char)

		if err != nil {
			return "", err
		}

		encodedData += encodedChar
	}

	return encodedData, nil
}

func decodePolybiusSquare(encodedData string, sourceCode string) (string, error) {
	if !isCodeValid(sourceCode) {
		return "", errors.New("Code is not valid")
	}

	code := prepareData(sourceCode)

	fmt.Println("[DE] Data to decode:", encodedData)
	fmt.Println("[DE] Prepared code:", code)
	square := polybiusSquare{}
	square.init(code)
	// square.print()

	decodedData := ""

	for _, char := range encodedData {
		encodedChar, err := square.decode(char)

		if err != nil {
			return "", err
		}

		decodedData += encodedChar
	}

	return decodedData, nil
}

func Run() {
	println("LB 1")

	sourceText := "Modern cryptography is heavily based on mathematical theory and computer science practice; cryptographic algorithms are designed around computational hardness assumptions, making such algorithms hard to break in actual practice by any adversary. While it is theoretically possible to break into a well-designed system, it is infeasible in actual practice to do so. Such schemes, if well designed, are therefore termed \"computationally secure\". Theoretical advances (e.g., improvements in integer factorization algorithms) and faster computing technology require these designs to be continually reevaluated and, if necessary, adapted. Information-theoretically secure schemes that provably cannot be broken even with unlimited computing power, such as the one-time pad, are much more difficult to use in practice than the best theoretically breakable but computationally secure schemes."
	code := "MODERN"

	fmt.Println("Source text:", sourceText)
	fmt.Println("Source code:", code)
	fmt.Println()

	encodedText, err := encodePolybiusSquare(sourceText, code)

	if err != nil {
		fmt.Println("Error in encodePolybiusSquare:", err)
		panic(err)
	}

	fmt.Println("Encoded text:", encodedText)

	fmt.Println()
	decodedText, err := decodePolybiusSquare(encodedText, code)

	if err != nil {
		fmt.Println("Error in decodePolybiusSquare:", err)
		panic(err)
	}

	fmt.Println("Decoded text:", decodedText)
	fmt.Println()
}
