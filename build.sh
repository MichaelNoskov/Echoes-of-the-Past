PROJECT="EchoesOfThePast"

echo "=============== Building ==============="
bear -- cmake -S . -B build || {
	echo "Incorrect CMakeLists.txt" >&2
	exit 1
}
bear -- cmake --build build || {
	echo "Couldn't build project" >&2
	exit 1
}

echo "Success!"
./build/"$PROJECT" || {
	echo "Game closed" >&2
}
