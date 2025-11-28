package lb4

import (
	"fmt"
	"io"
	"log"
	"math"
	"math/big"
)

const DEBUG = false

var lookUpTable map[rune]int = map[rune]int{
	'А': 1, 'Б': 2, 'В': 3, 'Г': 4, 'Д': 5,
	'Е': 6, 'Є': 7, 'Ж': 8, 'З': 9, 'И': 10,
	'І': 11, 'Ї': 12, 'Й': 13, 'К': 14, 'Л': 15,
	'М': 16, 'Н': 17, 'О': 18, 'П': 19, 'Р': 20,
	'С': 21, 'Т': 22, 'У': 23, 'Ф': 24, 'Х': 25,
	'Ц': 26, 'Ч': 27, 'Ш': 28, 'Щ': 29, 'Ь': 30,
	'Ю': 31, 'Я': 32, '=': 63, '_': 0,
}

// Golang math library works only with float64 :(
func powInt(x, y int) int {
	return int(math.Pow(float64(x), float64(y)))
}

func hashMessage(inputMessage string) int {
	message := []rune(inputMessage)
	hash := 9 // H0
	MOD := 33

	log.Printf("Index\tNum\tChar\tSum\tPower\tHash\n")

	for i, char := range message {
		symbol, ok := lookUpTable[char]

		if !ok {
			panic(fmt.Errorf("Symbol not found in lookUpTable! %d", symbol))
		}

		sum := (hash + symbol)
		power := sum * sum
		hash = power % MOD

		log.Printf("%d\t%d\t%s\t%d\t%d\t%d\n", i, symbol, string(char), sum, power, hash)
	}

	return hash
}

func hashMessageWithoutTable(message []int) int {
	hash := 9 // H0
	MOD := 33

	log.Printf("Index\tNum\tSum\tPower\tHash\n")

	for i, value := range message {
		sum := (hash + value)
		power := sum * sum
		hash = power % MOD
		hash = (hash + value) * power % MOD

		log.Printf("%d\t%d\t%d\t%d\t%d\n", i, value, sum, power, hash)
	}

	return hash
}

func signingRSA(h int, N, d int) int {
	S := powInt(h, d) % N

	return S
}

func verifyRSA(h int, s int, N, e int) bool {
	log.Println("Pow s^e%N is ", powInt(s, e)%N, "\t Hash is ", h)

	return powInt(s, e)%N == h
}

func openKeyElGamal(p, a, x int) int {
	return powInt(a, x) % p
}

func signingElGamal(hash int, p, a, x int) (int, int) {
	// k should be generated, but it's ok for lab work
	k := 13

	r := powInt(a, k) % p

	// Usage of math/big here because of "Euclidean modulus" in big.Mod
	pBig := big.NewInt(int64(p - 1))
	s := int(
		new(big.Int).Mod(
			new(big.Int).Mul(
				big.NewInt(int64(hash-x*r)),
				new(big.Int).ModInverse(
					big.NewInt(int64(k)),
					pBig,
				),
			),
			pBig,
		).Int64(),
	)

	if s == 0 {
		// Reference: https://en.wikipedia.org/wiki/ElGamal_signature_scheme
		panic("signingElGamal have S equal to 0, try different k")
	}

	return r, s
}

func verifyElGamal(hash int, r, s int, p, a, y int) bool {
	hBig := big.NewInt(int64(hash))
	rBig := big.NewInt(int64(r))
	sBig := big.NewInt(int64(s))
	pBig := big.NewInt(int64(p))
	aBig := big.NewInt(int64(a))
	yBig := big.NewInt(int64(y))

	first := new(big.Int).Exp(aBig, hBig, pBig)
	second := new(big.Int).Mod(
		new(big.Int).Mul(
			new(big.Int).Exp(yBig, rBig, nil),
			new(big.Int).Exp(rBig, sBig, nil),
		),
		pBig,
	)

	log.Println("Verify ElGamal", first, second)

	return first.Cmp(second) == 0
}

func Run() {
	if DEBUG {
		log.SetFlags(log.Lmsgprefix)
		log.SetPrefix("[DEBUG]: ")
	} else {
		log.SetOutput(io.Discard)
	}

	fmt.Println("Lb 4:")

	message := "ВИДАТИ_ТРИ_МЛН_ГРН_ГОЛОВН_БУХГ_СТРІЖКО="
	fmt.Println("Input message:", message)

	fmt.Println()
	hash := hashMessage(message)
	fmt.Printf("Hash is %d, in binary is %06b\n", hash, hash)

	type RSA struct {
		N int
		e int
		d int
	}

	rsa := RSA{N: 33, e: 7, d: 3}

	fmt.Println()
	signedRSA := signingRSA(hash, rsa.N, rsa.d)
	verifiedRSA := verifyRSA(hash, signedRSA, rsa.N, rsa.e)
	fmt.Printf("Signed RSA: %d. Verification is %t\n", signedRSA, verifiedRSA)

	type ElGamal struct {
		p int
		a int
		x int
		y int
	}

	elGamal := ElGamal{
		p: 41,
		a: 6,
		x: 10,
	}
	elGamal.y = openKeyElGamal(elGamal.p, elGamal.a, elGamal.x)

	fmt.Println()
	r, s := signingElGamal(hash, elGamal.p, elGamal.a, elGamal.x)
	verifiedElGamal := verifyElGamal(hash, r, s, elGamal.p, elGamal.a, elGamal.y)
	fmt.Printf("Signed ElGamal: r = %d, s = %d. Verification is %t\n", r, s, verifiedElGamal)

	fmt.Println()
	secondMessage := []int{
		0b00101011,
		0b01101000,
		0b01110011,
		0b01001100,

		0b10001000,
		0b01000000,
		0b01000000,
		0b00001100,

		0b11100001,
		0b11000100,
		0b01001000,
		0b10101001,

		0b10111010,
		0b10101101,
		0b00110100,
		0b00111010,

		0b00000000,
		0b01101001,
		0b00000101,
		0b00011001,
	}

	secondRSASignature := secondMessage[len(secondMessage)-1]
	secondMessage = secondMessage[:len(secondMessage)-1]
	fmt.Printf("Message: %+v, Signature %d\n", secondMessage, secondRSASignature)

	secondRSA := RSA{
		e: 7,
		N: 33,
	}

	isSecondVerified := verifyRSA(
		hashMessageWithoutTable(secondMessage),
		secondRSASignature,
		secondRSA.N,
		secondRSA.e,
	)

	fmt.Printf("Is second verified: %t\n", isSecondVerified)
}
