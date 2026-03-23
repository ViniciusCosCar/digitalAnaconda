

function run(){
    ./main

}


function build(){
    g++ main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o main
}

function hot(){
    g++ main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o main
    ./main
}
