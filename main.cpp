#include <iostream>
#include "./Board.h"

int main(int argc, char** argv) {
    std::cout << "Arrancando proyecto 'noobie'...\n";

    Board board(true);
    board.printBitboard(board.getOccupiedBitBoard());
	board.makeMove({ 12, 28, Board::W_PAWN, 0 }); // e2-e4
    board.printBitboard(board.getOccupiedBitBoard());
    // TODO: Inicializa aquí tu motor/servicio. Ejemplo:
    // MotorAjedrez motor;
    // motor.Iniciar();

    std::cout << "Inicialización por defecto completada (sin motor).\n";
    return 0;
}