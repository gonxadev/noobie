#include "Board.h"

Board::Board(bool init) {
    if (init) {
        //White pieces
        pieces[W_PAWN] = 255ULL << 8;
        pieces[W_BISHOP] = 36ULL;
        pieces[W_KNIGHT] = 66ULL;
        pieces[W_TOWER] = 129ULL;
        pieces[W_KING] = 16ULL;
        pieces[W_QUEEN] = 8ULL;

        //Black pieces
        pieces[B_PAWN] = 255ULL << 48;
        pieces[B_BISHOP] = 36ULL << 56;
        pieces[B_KNIGHT] = 66ULL << 56;
        pieces[B_TOWER] = 129ULL << 56;
        pieces[B_KING] = 16ULL << 56;
        pieces[B_QUEEN] = 8ULL << 56;
    } else {
        //White pieces
        pieces[W_PAWN] = 0ULL;
        pieces[W_BISHOP] = 0ULL;
        pieces[W_KNIGHT] = 0ULL;
        pieces[W_TOWER] = 0ULL;
        pieces[W_KING] = 0ULL;
        pieces[W_QUEEN] = 0ULL;

        //Black pieces
        pieces[B_PAWN] = 0ULL;
        pieces[B_BISHOP] = 0ULL;
        pieces[B_KNIGHT] = 0ULL;
        pieces[B_TOWER] = 0ULL;
        pieces[B_KING] = 0ULL;
        pieces[B_QUEEN] = 0ULL;
    }
}

Board::Board(uint64_t w_pawn, uint64_t w_bishop, uint64_t w_knight, uint64_t w_tower, uint64_t w_king, uint64_t w_queen, 
             uint64_t b_pawn, uint64_t b_bishop, uint64_t b_knight, uint64_t b_tower, uint64_t b_king, uint64_t b_queen)
{
    pieces[W_PAWN] = w_pawn;
    pieces[W_KNIGHT] = w_knight;
    pieces[W_BISHOP] = w_bishop;
    pieces[W_TOWER] = w_tower;
    pieces[W_KING] = w_king;
    pieces[W_QUEEN] = w_queen;

    pieces[B_PAWN] = b_pawn;
    pieces[B_KNIGHT] = b_knight;
    pieces[B_BISHOP] = b_bishop;
    pieces[B_TOWER] = b_tower;
    pieces[B_KING] = b_king;
    pieces[B_QUEEN] = b_queen;
}

void Board::printBitboard(uint64_t bitboard) {
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            std::cout << ((bitboard & (1ULL << square)) ? "1 " : ". ");
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Board::print() {
    std::cout << "----------------\n";
    printBitboard(getOccupiedBitBoard());
}

bool Board::getBlackCastle() const {
    return blackCastleLeft || blackCastleRight;
}

bool Board::getLeftBlackCastle() const {
    return blackCastleLeft;
}

bool Board::getRightBlackCastle() const {
    return blackCastleRight;
}

bool Board::getWhiteCastle() const {
    return whiteCastleLeft || whiteCastleRight;
}

bool Board::getLeftWhiteCastle() const {
    return whiteCastleLeft;
}

bool Board::getRightWhiteCastle() const {
    return whiteCastleRight;
}

bool Board::isWhiteToMove() const{
    return whiteToMove;
}

void Board::setWhiteToMove(bool value) {
    whiteToMove = value;
}

int Board::getMoves() {
    return moves;
}

const uint64_t(&Board::getPieces() const)[12] {
    return pieces;
}

Move Board::getLastMove() const{
    return lastMove;
}

uint64_t Board::getBit(uint64_t board, int from) {
    return board & (1ULL << from);
}

uint64_t Board::setBit(uint64_t board, int to) {
    return board | (1ULL << to);
}

// Sets a bit to 0
uint64_t Board::clearBit(uint64_t board, int to) {
    return board & ~(1ULL << to);
}

// Gets the position of all pieces of a certain type and color
uint64_t Board::getBitboardFromType(int pieceType) const{
    return pieces[pieceType];
}

//
uint64_t Board::getWhiteBitBoard() const{
    return pieces[W_PAWN] | pieces[W_BISHOP] |
        pieces[W_KNIGHT] | pieces[W_TOWER] |
        pieces[W_QUEEN] | pieces[W_KING];
}

//
uint64_t Board::getBlackBitBoard() const{
    return pieces[B_PAWN] | pieces[B_BISHOP] |
        pieces[B_KNIGHT] | pieces[B_TOWER] |
        pieces[B_QUEEN] | pieces[B_KING];
}

uint64_t Board::getOccupiedBitBoard() const{
    return getBlackBitBoard() | getWhiteBitBoard();
}

bool Board::isStalemate() const {
    return stalemate;
}

void Board::setStalemate(bool value) {
    stalemate = value;
}

bool Board::makeMove(Move move) {
    if (!stalemate && !checkmate) {
        BoardState currentState;
        for (int i = 0; i < 12; i++) currentState.pieces[i] = pieces[i];
        currentState.whiteToMove = whiteToMove;
        currentState.whiteCastleLeft = whiteCastleLeft;
        currentState.whiteCastleRight = whiteCastleRight;
        currentState.blackCastleLeft = blackCastleLeft;
        currentState.blackCastleRight = blackCastleRight;
        currentState.lastMove = lastMove;
        currentState.stalemate = stalemate;
        currentState.moves = moves;

        history.push(currentState);

        int pieceIndex = getBoardIndexFromMoveGenerator(move.piece);
        int lastMovePieceIndex = getBoardIndexFromMoveGenerator(lastMove.piece);
        int promotionIndex = getBoardIndexFromMoveGenerator(move.promotion);

        uint64_t from_mask = 1ULL << move.from;
        uint64_t delete_mask = ~from_mask;
        uint64_t to_mask = 1ULL << move.to;
        uint64_t occupied = getOccupiedBitBoard();
        uint64_t oppPosition = whiteToMove ? getBlackBitBoard() : getWhiteBitBoard();

        // Mover la pieza en su posición correspondiente
        pieces[pieceIndex] |= to_mask;
        pieces[pieceIndex] &= ~from_mask;

        // Borrar la pieza que haya sido capturada
        int size = sizeof(pieces) / sizeof(pieces[0]);
        for (int i = 0; i < size; i++) {
            if (i != pieceIndex) {
                pieces[i] &= ~to_mask;
            }
        }

        // Comprobar captura en passant
        int direction = whiteToMove ? 8 : -8;
        if ((pieceIndex == W_PAWN || pieceIndex == B_PAWN) && 
            (move.to == (move.from + direction + 1) || move.to == (move.from + direction - 1))) {
            if ((lastMovePieceIndex == W_PAWN || lastMovePieceIndex == B_PAWN) && abs(lastMove.from - lastMove.to) == 16) {

                int passantSquare = lastMove.to;                        // Casilla donde se encuentra la pieza contraria
                int passantLandingSqu = lastMove.to + direction;        // Casilla a donde se mueve la pieza atacante
                uint64_t passantLndSquMask = 1ULL << passantLandingSqu; // Máscara de la casilla de llegada
                uint64_t passantSquMask = 1ULL << passantSquare;

                if ((abs(passantSquare - move.from) == 1) && // Casilla es adyacente
                    (occupied & passantSquMask) &&           // Casilla adyacente ocupada por enemigo
                    !(occupied & passantLndSquMask)) {       // Casilla de llegada libre   
                    for (int i = 0; i < size; i++) {
                        pieces[i] &= ~passantSquMask;
                    }
                }
            }
        }

        // Comprobar el enroque
        if (pieceIndex == W_KING) {
            if (move.from == 4 && move.to == 6 && whiteCastleRight) {
                uint64_t from_tower_mask = 1ULL << 7;
                uint64_t to_tower_mask = 1ULL << 5;
                pieces[W_TOWER] |= to_tower_mask;
                pieces[W_TOWER] &= ~from_tower_mask;
            }
            else if (move.from == 4 && move.to == 2 && whiteCastleLeft) {
                uint64_t from_tower_mask = 1ULL << 0;
                uint64_t to_tower_mask = 1ULL << 3;
                pieces[W_TOWER] |= to_tower_mask;
                pieces[W_TOWER] &= ~from_tower_mask;
            }
            whiteCastleLeft = whiteCastleRight = false;
        }
        else if (pieceIndex == B_KING) {
            if (move.from == 60 && move.to == 62 && blackCastleRight) {
                uint64_t from_tower_mask = 1ULL << 63;
                uint64_t to_tower_mask = 1ULL << 61;
                pieces[B_TOWER] |= to_tower_mask;
                pieces[B_TOWER] &= ~from_tower_mask;
            }
            else if (move.from == 60 && move.to == 57 && blackCastleLeft) {
                uint64_t from_tower_mask = 1ULL << 56;
                uint64_t to_tower_mask = 1ULL << 59;
                pieces[B_TOWER] |= to_tower_mask;
                pieces[B_TOWER] &= ~from_tower_mask;
            }
            blackCastleLeft = blackCastleRight = false;
        }
        else if (pieceIndex == W_TOWER && move.from == 0) {
            whiteCastleLeft = false;
        }
        else if (pieceIndex == W_TOWER && move.from == 7) {
            whiteCastleRight = false;
        }
        else if (pieceIndex == B_TOWER && move.from == 56) {
            blackCastleLeft = false;
        }
        else if (pieceIndex == B_TOWER && move.from == 63) {
            blackCastleRight = false;
        }

        // Comprobar si hay una promoción
        if ((pieceIndex == W_PAWN && move.to >= 56) || (pieceIndex == B_PAWN && move.to < 8)) {
            if (promotionIndex != -1) { // Si hay promoción
                pieces[promotionIndex] |= to_mask;
                pieces[pieceIndex] &= ~to_mask;
            }
        }

        // Actualizar la cuenta de movimientos y a quíen le toca mover y último mov
        moves++;
        whiteToMove = !whiteToMove;
        lastMove = move;
        return true;
    }
    return false;
}

void Board::unmakeMove() {
    if (history.empty()) return;

    BoardState previousState = history.top();
    history.pop();

    for (int i = 0; i < 12; i++) pieces[i] = previousState.pieces[i];
    whiteToMove = previousState.whiteToMove;
    whiteCastleLeft = previousState.whiteCastleLeft;
    whiteCastleRight = previousState.whiteCastleRight;
    blackCastleLeft = previousState.blackCastleLeft;
    blackCastleRight = previousState.blackCastleRight;
    lastMove = previousState.lastMove;
    stalemate = previousState.stalemate;
    moves = previousState.moves;
}

int Board::getBoardIndexFromMoveGenerator(int indexFromMoveGenerator) {
    int indexFromBoard;
    if (indexFromMoveGenerator == 0) {
        indexFromBoard = (whiteToMove ? W_PAWN : B_PAWN);
    }
    else if (indexFromMoveGenerator == 1) {
        indexFromBoard = (whiteToMove ? W_BISHOP : B_BISHOP);
    }
    else if (indexFromMoveGenerator == 2) {
        indexFromBoard = (whiteToMove ? W_KNIGHT : B_KNIGHT);
    }
    else if (indexFromMoveGenerator == 3) {
        indexFromBoard = (whiteToMove ? W_TOWER : B_TOWER);
    }
    else if (indexFromMoveGenerator == 4) {
        indexFromBoard = (whiteToMove ? W_QUEEN : B_QUEEN);
    }
    else if (indexFromMoveGenerator == 5) {
        indexFromBoard = (whiteToMove ? W_KING : B_KING);
    }
    else {
        indexFromBoard = -1;
    }
    return indexFromBoard;
}

int Board::getBitFromBoard(int pieceIndex, int from) {
    if (pieceIndex < 0 || pieceIndex >= 12) {
        throw std::out_of_range("Índice de pieza fuera de rango");
    }
    if (from < 0 || from >= 64) {
        throw std::out_of_range("Posición fuera de rango");
    }

    uint64_t position = pieces[pieceIndex];
    return (position >> from) & 1;
}

void Board::setBitOfBoard(int pieceIndex, int to) {
    if (pieceIndex < 0 || pieceIndex >= 12) {
        throw std::out_of_range("Índice de pieza fuera de rango");
    }
    if (to < 0 || to >= 64) {
        throw std::out_of_range("Posición fuera de rango");
    }

    pieces[pieceIndex] |= (1ULL << to);
}

void Board::clearBitOfBoard(int pieceIndex, int to) {
    if (pieceIndex < 0 || pieceIndex >= 12) {
        throw std::out_of_range("Índice de pieza fuera de rango");
    }
    if (to < 0 || to >= 64) {
        throw std::out_of_range("Posición fuera de rango");
    }

    pieces[pieceIndex] &= ~(1ULL << to); 
}

Board Board::clone() const {
    Board copy(false);
    for (int i = 0; i < 12; i++) {
        copy.pieces[i] = this->pieces[i];
    }
    copy.whiteCastleLeft = this->whiteCastleLeft;
    copy.whiteCastleRight = this->whiteCastleRight;
    copy.blackCastleLeft = this->blackCastleLeft;
    copy.blackCastleRight = this->blackCastleRight;
    copy.whiteToMove = this->whiteToMove;
    copy.moves = this->moves;
    copy.lastMove = this->lastMove;
    copy.stalemate = this->stalemate;

    return copy;
}