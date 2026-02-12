#include <iostream>
#include "./Board.h"

int main(int argc, char** argv) {

    Board board(true);
    board.printBitboard(board.getOccupiedBitBoard());
	board.makeMove({ 12, 28, Board::W_PAWN, 0 }); // e2-e4
    board.printBitboard(board.getOccupiedBitBoard());
    // TODO: Inicializa aquí tu motor/servicio. Ejemplo:
    // MotorAjedrez motor;
    // motor.Iniciar();

    return 0;
}