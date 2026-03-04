#include "Board.h"
#include "MoveGenerator.h"

Board::Board() {
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
}

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
    moveGen = std::make_unique<MoveGenerator>();
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

    moveGen = std::make_unique<MoveGenerator>();
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

void Board::updateStalemateCheckmateStatus() {
    int color = whiteToMove ? WHITE : BLACK;
    std::vector<Move> legalMoves = generateLegalMoves(color);
    if (legalMoves.empty()) {
        if (isKingInCheck(*this, color)) {
            checkmate = true;
        } else {
            stalemate = true;
        }
    }
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

uint64_t Board::getWhiteBitBoard() const{
    return pieces[W_PAWN] | pieces[W_BISHOP] |
        pieces[W_KNIGHT] | pieces[W_TOWER] |
        pieces[W_QUEEN] | pieces[W_KING];
}

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

        // En passant
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

        // Castling
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

        // Check for a promotion
        if ((pieceIndex == W_PAWN && move.to >= 56) || (pieceIndex == B_PAWN && move.to < 8)) {
            if (promotionIndex != -1) { // There's a promotion
                pieces[promotionIndex] |= to_mask;
                pieces[pieceIndex] &= ~to_mask;
            }
        }

        // Update move count, turn and last move
        moves++;
        whiteToMove = !whiteToMove;
        lastMove = move;
        return true;
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

int Board::getBoardIndexFromMoveGenerator(int indexFromMoveGenerator) const {
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

bool Board::isSameDiagonal(int from, int to)
{
    int from_rank = from / 8;
    int from_col = from % 8;

    int to_rank = to / 8;
    int to_col = to % 8;

    return std::abs(from_rank - to_rank) == std::abs(from_col - to_col);
}

bool Board::isKnigthMove(int from, int to)
{
    bool isKnightMove = false;
    if (!(to > 63 || to < 0)) {
        int from_rank = from / 8;
        int from_col = from % 8;

        int to_rank = to / 8;
        int to_col = to % 8;

        int rank_diff = std::abs(from_rank - to_rank);
        int col_diff = std::abs(from_col - to_col);

        if ((rank_diff == 1 && col_diff == 2) || rank_diff == 2 && col_diff == 1) {
            isKnightMove = true;
        }
    }
    return isKnightMove;
}

bool Board::isSameRank(int from, int to)
{
    int from_rank = from / 8;
    int to_rank = to / 8;
    return (from_rank == to_rank);
}

bool Board::isSameColumn(int from, int to)
{
    int from_col = from % 8;
    int to_col = to % 8;
    return (from_col == to_col);
}

bool Board::isHorizontalPathClear(Move m, uint64_t occupied) {
    bool isClear = true;

    int fromRank = m.from / 8;
    int toRank = m.to / 8;

    if (fromRank != toRank) {
        throw std::invalid_argument("Movimiento no válido: debe ser horizontal.");
    }

    if (m.from == m.to) {
        throw std::invalid_argument("La casilla de origen y destino no pueden ser la misma.");
    }

    if (m.from > m.to) {
        for (int i = m.from - 1; i > m.to; i--) {
            uint64_t i_mask = 1ULL << i;
            if (i_mask & occupied) {
                isClear = false;
                break;
            }
        }
    }
    else {
        for (int i = m.from + 1; i < m.to; i++) {
            uint64_t i_mask = 1ULL << i;
            if (i_mask & occupied) {
                isClear = false;
                break;
            }
        }
    }
    return isClear;
}

bool Board::isVerticalPathClear(Move m, uint64_t occupied) {
    bool isClear = true;
    int fromRank = m.from / 8;
    int fromCol = m.from % 8;
    int toRank = m.to / 8;
    int toCol = m.to % 8;

    if (m.from == m.to || fromCol != toCol) {
        throw std::invalid_argument("Movimiento no válido: debe ser vertical.");
    }

    if (m.from == m.to) {
        throw std::invalid_argument("La casilla de origen y destino no pueden ser la misma.");
    }

    if (fromRank > toRank) {
        for (int rank = fromRank - 1; rank > toRank; rank--) {
            uint64_t mask = 1ULL << (rank * 8 + fromCol);
            if (mask & occupied) {
                isClear = false;
                break;
            }
        }
    }
    else {
        for (int rank = fromRank + 1; rank < toRank; rank++) {
            uint64_t mask = 1ULL << (rank * 8 + fromCol);
            if (mask & occupied) {
                isClear = false;
                break;
            }
        }
    }
    return isClear;
}

bool Board::isDiagonalPathClear(Move m, uint64_t occupied) {
    bool isClear = true;
    int fromRank = m.from / 8;
    int fromCol = m.from % 8;
    int toRank = m.to / 8;
    int toCol = m.to % 8;

    if (m.from == m.to || abs(fromRank - toRank) != abs(fromCol - toCol)) {
        throw std::invalid_argument("Movimiento no válido: debe ser diagonal.");
    }

    if (m.from == m.to) {
        throw std::invalid_argument("La casilla de origen y destino no pueden ser la misma.");
    }

    int rankStep = (toRank > fromRank) ? 1 : -1;
    int colStep = (toCol > fromCol) ? 1 : -1;

    for (int rank = fromRank + rankStep, col = fromCol + colStep;
        rank != toRank;
        rank += rankStep, col += colStep) {
        uint64_t mask = 1ULL << (rank * 8 + col);
        if (mask & occupied) {
            isClear = false;
            break;
        }
    }
    return isClear;
}

bool Board::isSquareUnderAttack(const Board& board, int square, int attackerColor) {
    if (square > 63 || square < 0) {
        throw std::invalid_argument("square debe ser menor-igual de 63, o mayor-igual de 0");
    }
    if (attackerColor != Board::WHITE && attackerColor != Board::BLACK) {
        throw std::invalid_argument("color debe ser 0 (WHITE) o 1 (NEGRO)");
    }
    uint64_t squareMask = 1ULL << square;

    uint64_t position = (attackerColor == WHITE) ? board.getWhiteBitBoard()
        : board.getBlackBitBoard();

    uint64_t occupied = board.getOccupiedBitBoard();

    int oppColor = (attackerColor == BLACK) ? Board::WHITE
        : Board::BLACK;

    uint64_t enemyPawns = board.getBitboardFromType((attackerColor == Board::WHITE) ? Board::W_PAWN : Board::B_PAWN);
    uint64_t enemyBishop = board.getBitboardFromType((attackerColor == Board::WHITE) ? Board::W_BISHOP : Board::B_BISHOP);
    uint64_t enemyKnight = board.getBitboardFromType((attackerColor == Board::WHITE) ? Board::W_KNIGHT : Board::B_KNIGHT);
    uint64_t enemyTower = board.getBitboardFromType((attackerColor == Board::WHITE) ? Board::W_TOWER : Board::B_TOWER);
    uint64_t enemyQueen = board.getBitboardFromType((attackerColor == Board::WHITE) ? Board::W_QUEEN : Board::B_QUEEN);
    uint64_t enemyKing = board.getBitboardFromType((attackerColor == Board::WHITE) ? Board::W_KING : Board::B_KING);

    uint64_t attacks = 0ULL;
    attacks |= generatePawnAttacks(enemyPawns, oppColor);
    attacks |= generateBishopAttacks(enemyBishop, position, occupied);
    attacks |= generateKnightAttacks(enemyKnight, position, occupied);
    attacks |= generateTowerAttacks(enemyTower, position, occupied);
    attacks |= generateQueenAttacks(enemyQueen, position, occupied);
    attacks |= generateKingAttacks(enemyKing, position, occupied);

    if (squareMask & attacks) {
        return true;
    }

    return false;
}

uint64_t Board::generatePawnAttacks(uint64_t pawns, int color) {
    uint64_t attacks = 0ULL;
    if (color == Board::WHITE) {
        attacks |= (pawns & ~Board::left) << 7; // Evita salirse por la columna A
        attacks |= (pawns & ~Board::right) << 9; // Evita salirse por la columna H
    }
    else {
        attacks |= (pawns & ~Board::left) >> 7; // Evita salirse por la columna A
        attacks |= (pawns & ~Board::right) >> 9; // Evita salirse por la columna H
    }
    return attacks;
}

uint64_t Board::generateBishopAttacks(uint64_t bishops, uint64_t opposition, uint64_t occupied) {
    uint64_t attacks = 0ULL;

    for (int square = 0; square < 64; square++) {

        if (!(bishops & (1ULL << square)))
            continue;

        int rank = square / 8;
        int file = square % 8;

        // arriba-derecha (+9)
        for (int r = rank + 1, f = file + 1; r < 8 && f < 8; r++, f++) {
            int to = r * 8 + f;
            uint64_t mask = 1ULL << to;
            attacks |= mask;
            if (mask & occupied) break;
        }

        // abajo-izquierda (-9)
        for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--) {
            int to = r * 8 + f;
            uint64_t mask = 1ULL << to;
            attacks |= mask;
            if (mask & occupied) break;
        }

        // arriba-izquierda (+7)
        for (int r = rank + 1, f = file - 1; r < 8 && f >= 0; r++, f--) {
            int to = r * 8 + f;
            uint64_t mask = 1ULL << to;
            attacks |= mask;
            if (mask & occupied) break;
        }

        // abajo-derecha (-7)
        for (int r = rank - 1, f = file + 1; r >= 0 && f < 8; r--, f++) {
            int to = r * 8 + f;
            uint64_t mask = 1ULL << to;
            attacks |= mask;
            if (mask & occupied) break;
        }
    }

    return attacks;
}

uint64_t Board::generateKnightAttacks(uint64_t knights, uint64_t opposition, uint64_t occupied) {
    uint64_t attacks = 0ULL;
    for (int square = 0; square < 64; square++) {
        if (knights & (1ULL << square)) {
            // 2 casillas a la izquierda + 1 hacia arriba (mueve hacia la izquierda)
            if (square % 8 > 1) {  // Verificar si está en el borde izquierdo
                if (square > 15) attacks |= (1ULL << (square - 17)); // Arriba izquierda
                if (square < 48) attacks |= (1ULL << (square + 15)); // Abajo izquierda
            }
            // 2 casillas a la derecha + 1 hacia arriba
            if (square % 8 < 6) {  // Verificar si está en el borde derecho
                if (square > 15) attacks |= (1ULL << (square - 15)); // Arriba derecha
                if (square < 48) attacks |= (1ULL << (square + 17)); // Abajo derecha
            }
            // 2 casillas hacia arriba + 1 hacia la derecha
            if (square > 15) {
                if (square % 8 > 0) attacks |= (1ULL << (square - 6)); // Arriba derecha
                if (square % 8 < 7) attacks |= (1ULL << (square - 10)); // Arriba izquierda
            }
            // 2 casillas hacia abajo + 1 hacia la derecha
            if (square < 48) {
                if (square % 8 > 0) attacks |= (1ULL << (square + 6)); // Abajo derecha
                if (square % 8 < 7) attacks |= (1ULL << (square + 10)); // Abajo izquierda
            }
        }
    }
    return attacks;
}

uint64_t Board::generateTowerAttacks(uint64_t towers, uint64_t opposition, uint64_t occupied) {
    uint64_t attacks = 0ULL;

    for (int square = 0; square < 64; square++) {

        if ((1ULL << square) & towers) {
            int fromRank = square / 8;
            // Hacia la derecha
            if (!((1ULL << square) & Board::right)) { // Límite derecho del tablero
                for (int to = (square + 1); ((to / 8) == fromRank) && (to < 64); to++) {
                    uint64_t attack = 1ULL << to;

                    if (!(attack & occupied)) {
                        attacks |= attack;
                    }
                    else {
                        if (attack & opposition) {
                            attacks |= attack;
                        }
                        break;
                    }
                }
            }

            // Hacia la izquierda
            if (!((1ULL << square) & Board::left)) { // Límite izquierdo del tablero
                for (int to = (square - 1); ((to / 8) == fromRank) && (to >= 0); to--) {
                    uint64_t attack = 1ULL << to;
                    if (!(attack & occupied)) {
                        attacks |= attack;
                    }
                    else {
                        if (attack & opposition) {
                            attacks |= attack;
                        }
                        break;
                    }
                }
            }

            // Hacia arriba
            if (!((1ULL << square) & Board::top)) {
                for (int to = (square + 8); to < 64; to += 8) {
                    uint64_t attack = 1ULL << to;
                    if (!(attack & occupied)) {
                        attacks |= attack;
                    }
                    else {
                        if (attack & opposition) {
                            attacks |= attack;
                        }
                        break;
                    }
                }
            }

            // Hacia abajo
            if (!((1ULL << square) & Board::bottom)) {
                for (int to = (square - 8); to >= 0; to -= 8) {
                    uint64_t attack = 1ULL << to;
                    if (!(attack & occupied)) {
                        attacks |= attack;
                    }
                    else {
                        if (attack & opposition) {
                            attacks |= attack;
                        }
                        break;
                    }
                }
            }
        }
    }
    return attacks;
}

uint64_t Board::generateQueenAttacks(uint64_t queens, uint64_t opposition, uint64_t occupied) {
    uint64_t attacks = 0ULL;
    for (int square = 0; square < 64; square++) {
        if (!((1ULL << square) && queens)) {
            attacks |= generateBishopAttacks(queens, opposition, occupied);
            attacks |= generateTowerAttacks(queens, opposition, occupied);
        }
    }
    return attacks;
}

uint64_t Board::generateKingAttacks(uint64_t king, uint64_t opposition, uint64_t occupied) {
    uint64_t attacks = 0ULL;
    for (int square = 0; square < 64; square++) {
        if ((1ULL << square) & king) {
            // Arriba
            uint64_t attack;
            if (!((1ULL << square) & Board::top)) {
                attack = (1ULL << (square + 8));
                if (!(attack & occupied)) {
                    attacks |= attack;
                }
                else {
                    if (attack & opposition) {
                        attacks |= attack;
                    }
                }
                // Arriba derecha
                if (!((1ULL << square) & Board::right)) {
                    attack = (1ULL << (square + 9));
                    if (!(attack & occupied)) {
                        attacks |= attack;
                    }
                    else {
                        if (attack & opposition) {
                            attacks |= attack;
                        }
                    }
                }
                // Arriba izquierda
                if (!((1ULL << square) & Board::left)) {
                    attacks |= (1ULL << (square + 7));
                    attack = (1ULL << (square + 7));
                    if (!(attack & occupied)) {
                        attacks |= attack;
                    }
                    else {
                        if (attack & opposition) {
                            attacks |= attack;
                        }
                    }
                }
            }
            // Derecha
            if (!((1ULL << square) & Board::right)) {
                attack = (1ULL << (square + 1));
                if (!(attack & occupied)) {
                    attacks |= attack;
                }
                else {
                    if (attack & opposition) {
                        attacks |= attack;
                    }
                }
            }
            // Abajo
            if (!((1ULL << square) & Board::bottom)) {
                attack = (1ULL << (square - 8));
                if (!(attack & occupied)) {
                    attacks |= attack;
                }
                else {
                    if (attack & opposition) {
                        attacks |= attack;
                    }
                }
                // Abajo derecha
                if (!((1ULL << square) & Board::right)) {
                    attack = (1ULL << (square - 7));
                    if (!(attack & occupied)) {
                        attacks |= attack;
                    }
                    else {
                        if (attack & opposition) {
                            attacks |= attack;
                        }
                    }
                }
                // Abajo izquierda
                if (!((1ULL << square) & Board::left)) {
                    attack = (1ULL << (square - 9));
                    if (!(attack & occupied)) {
                        attacks |= attack;
                    }
                    else {
                        if (attack & opposition) {
                            attacks |= attack;
                        }
                    }
                }
            }
            // Izquierda
            if (!((1ULL << square) & Board::left)) {
                attack = (1ULL << (square - 1));
                if (!(attack & occupied)) {
                    attacks |= attack;
                }
                else {
                    if (attack & opposition) {
                        attacks |= attack;
                    }
                }
            }
        }
    }
    return attacks;
}

bool Board::isKingInCheck(const Board& board, int kingColor) {
    uint64_t kingBitboard = (kingColor == WHITE) ? board.getBitboardFromType(Board::W_KING)
        : board.getBitboardFromType(Board::B_KING);

    uint64_t position = (kingColor == WHITE) ? board.getWhiteBitBoard()
        : board.getBlackBitBoard();

    uint64_t occupied = board.getOccupiedBitBoard();

    int oppColor = (kingColor == WHITE) ? BLACK : WHITE;

    // Buscamos la posición del rey
    int kingPosition = -1;
    for (int i = 0; i < 64; i++) {
        if (kingBitboard & (1ULL << i)) {
            kingPosition = i;
            break;
        }
    }

    uint64_t enemyPawns = board.getBitboardFromType((oppColor == Board::WHITE) ? Board::W_PAWN : Board::B_PAWN);
    uint64_t enemyBishop = board.getBitboardFromType((oppColor == Board::WHITE) ? Board::W_BISHOP : Board::B_BISHOP);
    uint64_t enemyKnight = board.getBitboardFromType((oppColor == Board::WHITE) ? Board::W_KNIGHT : Board::B_KNIGHT);
    uint64_t enemyTower = board.getBitboardFromType((oppColor == Board::WHITE) ? Board::W_TOWER : Board::B_TOWER);
    uint64_t enemyQueen = board.getBitboardFromType((oppColor == Board::WHITE) ? Board::W_QUEEN : Board::B_QUEEN);
    uint64_t enemyKing = board.getBitboardFromType((oppColor == Board::WHITE) ? Board::W_KING : Board::B_KING);

    uint64_t attacks = 0ULL;
    attacks |= generatePawnAttacks(enemyPawns, oppColor);
    attacks |= generateBishopAttacks(enemyBishop, position, occupied);
    attacks |= generateKnightAttacks(enemyKnight, position, occupied);
    attacks |= generateKnightAttacks(enemyTower, position, occupied);
    attacks |= generateKnightAttacks(enemyQueen, position, occupied);
    attacks |= generateKnightAttacks(enemyKing, position, occupied);

    if (kingBitboard & attacks) {
        return true;
    }

    return false;
}

bool Board::isLegal(const Board& board, Move m, int color) {
    int oppColor = ((color == WHITE) ? BLACK : WHITE);
    int pieceIndex = board.getBoardIndexFromMoveGenerator(m.piece);
    uint64_t occupied = board.getOccupiedBitBoard();
    uint64_t position = (board.isWhiteToMove() ? board.getWhiteBitBoard() : board.getBlackBitBoard());
    uint64_t oppPosition = (board.isWhiteToMove() ? board.getBlackBitBoard() : board.getWhiteBitBoard());
    uint64_t toMask = 1ULL << m.to;
    Move lastMove = board.getLastMove();
	int lastMovePieceIndex = board.getBoardIndexFromMoveGenerator(lastMove.piece);

    // Verificar si la casilla de destino está ocupada por una pieza del mismo color
    if (toMask & position) {
        return false;
    }

    // Comprobar si el camino está despejado según el tipo de movimiento
    if ((pieceIndex == Board::W_TOWER || pieceIndex == Board::B_TOWER)) {
        if (isSameRank(m.from, m.to)) {
            if (!isHorizontalPathClear(m, occupied))
                return false;
        }
        else if (isSameColumn(m.from, m.to)) {
            if (!isVerticalPathClear(m, occupied))
                return false;
        }
        else {
            return false; 
        }
    }
    else if (pieceIndex == Board::W_QUEEN || pieceIndex == Board::B_QUEEN) {
        if (isSameRank(m.from, m.to)) {
            if (!isHorizontalPathClear(m, occupied))
                return false;
        }
        else if (isSameColumn(m.from, m.to)) {
            if (!isVerticalPathClear(m, occupied))
                return false;
        }
        else if (isSameDiagonal(m.from, m.to)) {
            if (!isDiagonalPathClear(m, occupied))
                return false;
        }
        else {
            return false; 
        }
    }
    else if ((pieceIndex == Board::W_BISHOP || pieceIndex == Board::B_BISHOP)) {
        if (isSameDiagonal(m.from, m.to)) {
            if (!isDiagonalPathClear(m, occupied))
                return false;
        }
        else {
            return false; 
        }
    }
    else if ((pieceIndex == Board::W_KNIGHT || pieceIndex == Board::B_KNIGHT) && !(Board::isKnigthMove(m.from, m.to))) { // Comprobación para los caballos
        return false;
    }
    else if (pieceIndex == Board:: W_PAWN || pieceIndex == Board::B_PAWN) {
        int direction = (color == Board::WHITE) ? 8 : -8; // Blancas suben, negras bajan
        // int startRow = (board.isWhiteToMove()) ? 8 : 48;  // Filas iniciales para peones
        int startRow = m.from / 8;
        //int startRank = m.from % 8;

        // 1 casillas hacia delante
        if ((m.to == m.from + direction) && !(toMask & occupied)) {
            goto CHECK_KING;
        }

        // 2 casillas hacia delante
        if ((color == WHITE && startRow == 1) || (color == BLACK && startRow == 6)) {
            uint64_t intermediateSqu = 1ULL << (m.from + direction);
            if (m.to == m.from + 2 * direction && !(toMask & occupied) && !(intermediateSqu & occupied)) {
                goto CHECK_KING;
            }
        }

        // Movimiento en diagonal
        if ((m.to == (m.from + direction + 1)) || (m.to == (m.from + direction - 1))) {
            // Captura en diagonal
            if (toMask & oppPosition) {
                goto CHECK_KING;
            }
            // En passant
            else if ((lastMovePieceIndex == Board::W_PAWN || lastMovePieceIndex == Board::B_PAWN) 
                && abs(lastMove.from - lastMove.to) == 16) { // Anterior movimiento es de peón, de dos casillas
                int passantSquare = lastMove.to; // Casilla donde se encuentra la pieza contraria
                int passantLandingSqu = lastMove.to + direction; // Casilla a donde debiera de moverse la pieza atacante para capturar en passant
                if (m.to == passantLandingSqu && (abs(passantSquare - m.from) == 1)) { // Casilla adyacente
                    goto CHECK_KING;
                }
            }
        }
        return false;
    }
    else if (pieceIndex == Board::W_KING || pieceIndex == Board::B_KING) {
        int diff = abs(m.from - m.to);
        if (diff == 1 || diff == 9 || diff == 7 || diff == 8) {
            goto CHECK_KING;
        }

        if (((color == WHITE) && (m.from == 4) && board.getWhiteCastle())) {
            if (Board::isSameRank(m.from, m.to) && (diff == 2)) { // El rey se mueve dos a la izq o der
                if (Board::isHorizontalPathClear(m, occupied)) {
                    if (m.from < m.to && !(isSquareUnderAttack(board, m.from + 1, oppColor)) && board.getRightWhiteCastle()) {
                        goto CHECK_KING;
                    }
                    if (m.from > m.to && !(isSquareUnderAttack(board, m.from - 1, oppColor)) && board.getLeftWhiteCastle()) {
                        goto CHECK_KING;
                    }
                }
            }
        }
        else if (((color == BLACK) && (m.from == 60) && board.getBlackCastle())) {
            if (Board::isSameRank(m.from, m.to) && (diff == 2)) { // El rey se mueve dos a la izq o der
                if (Board::isHorizontalPathClear(m, occupied)) {
                    if (m.from < m.to && !(isSquareUnderAttack(board, m.from + 1, oppColor)) && board.getRightBlackCastle()) {
                        goto CHECK_KING;
                    }
                    if (m.from > m.to && !(isSquareUnderAttack(board, m.from - 1, oppColor)) && board.getLeftBlackCastle()) {
                        goto CHECK_KING;
                    }
                }
            }
        }
        return false;
    }

CHECK_KING:
    // Tabla auxiliar para comprobar el jaque
    Board auxBoard = board.clone();
    auxBoard.makeMove(m);

    return !isKingInCheck(auxBoard, color);
}

std::vector<Move> Board::generateLegalMoves(int color) {
	std::vector<Move> moves = this->moveGen->generateMoves(*this, color);
    /*
    for (int i = 0; i < moves.size(); i++) {
        Move move = moves.at(i);
        if (!isLegal(*this, move, color)) {
            moves.erase(moves.begin() + i);
            i--;
        }
    }
    */
    moves.erase(std::remove_if(moves.begin(), moves.end(),
        [&](const Move& m) { return !isLegal(*this, m, color); }),
        moves.end());
    return moves;
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