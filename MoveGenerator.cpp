#include "MoveGenerator.h"

std::vector<Move> MoveGenerator::generateMoves(const Board& board, int color) 
{
    std::vector<Move> moves;
    generatePawnMoves(board, moves, color);
    generateBishopMoves(board, moves, color, false);
    generateKnightMoves(board, moves, color);
    generateTowerMoves(board, moves, color, false);
    generateQueenMoves(board, moves, color);
    generateKingMoves(board, moves, color);

    filterMoves(board, moves, color);

    moves.shrink_to_fit();
    return moves;
}

bool MoveGenerator::isSameDiagonal(int from, int to) 
{
    int from_rank = from / 8;
    int from_col = from % 8;

    int to_rank = to / 8;
    int to_col = to % 8;

    return std::abs(from_rank - to_rank) == std::abs(from_col - to_col);
}

bool MoveGenerator::isKnigthMove(int from, int to) 
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

bool MoveGenerator::isSameRank(int from, int to)
{
    int from_rank = from / 8;
    int to_rank = to / 8;
    return (from_rank == to_rank);
}

bool MoveGenerator::isSameColumn(int from, int to)
{
    int from_col = from % 8;
    int to_col = to % 8;
    return (from_col == to_col);
}

void MoveGenerator::generatePawnMoves(const Board& board, std::vector<Move>& moves, int color) 
{
    if (color == WHITE) {
        generateWhitePawnsMoves(board, moves);
    } else {
        generateBlackPawnMoves(board, moves);
    }
}

void MoveGenerator::generateWhitePawnsMoves(const Board& board, std::vector<Move>& moves) 
{
    uint64_t w_pawns = board.getBitboardFromType(Board::W_PAWN);

    for (int from = 0; from < 64; from++) {
        uint64_t from_mask = Board::getBit(w_pawns, from);
        if (from_mask & w_pawns) {
            int from_rank = from / 8;
            int from_col = from % 8;
            
            Move fwd_1{ from, from + 8, PAWN, NULL_TYPE };
            moves.push_back(fwd_1);

            if (from_rank == 1) {
                Move fwd_2{ from, from + 16, PAWN, NULL_TYPE };
                moves.push_back(fwd_2);
            }
            if (from_col < 7) {
                Move right{ from, from + 9, PAWN, NULL_TYPE };
                moves.push_back(right);
            }
            if (from_col > 0) {
                Move left{ from, from + 7, PAWN, NULL_TYPE };
                moves.push_back(left);
            }
        }
    }
}

void MoveGenerator::generateBlackPawnMoves(const Board& board, std::vector<Move>& moves)
{
    uint64_t b_pawns = board.getBitboardFromType(board.B_PAWN);

    for (int from = 0; from < 64; from++) {
        uint64_t from_mask = Board::getBit(b_pawns, from);
        if (from_mask & b_pawns) {
            int from_rank = from / 8;
            int from_col = from % 8;

            Move fwd_1{ from, from - 8, PAWN, NULL_TYPE };
            moves.push_back(fwd_1);

            if (from_rank == 6) {
                Move fwd_2{ from, from - 16, PAWN, NULL_TYPE };
                moves.push_back(fwd_2);
            }

            if (from_col < 7) {
                Move right{ from, from - 7, PAWN, NULL_TYPE };
                moves.push_back(right);
            }

            if (from_col > 0) {
                Move left{ from, from - 9, PAWN, NULL_TYPE };
                moves.push_back(left);
            }
        }
    }
}

void MoveGenerator::generateBishopMoves(const Board& board, std::vector<Move>& moves, int color, bool forQueen) 
{
    uint64_t bishops;

    if (forQueen) {
        if (color == WHITE) {
            bishops = board.getBitboardFromType(Board::W_QUEEN);
        }
        else if (color == BLACK) {
            bishops = board.getBitboardFromType(Board::B_QUEEN);
        }
    } 
    else 
    {
        if (color == WHITE) {
            bishops = board.getBitboardFromType(Board::W_BISHOP);
        }
        else if (color == BLACK) {
            bishops = board.getBitboardFromType(Board::B_BISHOP);
        }
    }
    

    for (int from = 0; from < 64; from++) {
        uint64_t from_mask = 1ULL << from;
        if (from_mask & bishops) {
            for (int to = from + 9; to < 64; to += 9) {
                if (!MoveGenerator::isSameDiagonal(from, to)) {
                    break;
                }
                Move m{ from, to, (forQueen ? QUEEN : BISHOP), NULL_TYPE };
                moves.push_back(m);
            }
            for (int to = from - 9; to >= 0; to -= 9) {
                if (!MoveGenerator::isSameDiagonal(from, to)) {
                    break;
                }
                Move m{ from, to, (forQueen ? QUEEN : BISHOP), NULL_TYPE };
                moves.push_back(m);
            }
            for (int to = from + 7; to < 64; to += 7) {
                if (!MoveGenerator::isSameDiagonal(from, to)) {
                    break;
                }
                Move m{ from, to, (forQueen ? QUEEN : BISHOP), NULL_TYPE };
                moves.push_back(m);
            }
            for (int to = from - 7; to >= 0; to -= 7) {
                if (!MoveGenerator::isSameDiagonal(from, to)) {
                    break;
                }
                Move m{ from, to, (forQueen ? QUEEN : BISHOP), NULL_TYPE };
                moves.push_back(m);
            }
        }
    }
}

void MoveGenerator::generateKnightMoves(const Board& board, std::vector<Move>& moves, int color) 
{
    uint64_t knight;
    if (color == WHITE) {
        knight = board.getBitboardFromType(Board::W_KNIGHT);
    }
    else if (color == BLACK) {
        knight = board.getBitboardFromType(Board::B_KNIGHT);
    }

    for (int from = 0; from < 64; from++) {
        uint64_t from_mask= 1ULL << from;
        if (from_mask & knight) {
            int from_rank = from / 8;
            int from_col = from % 8;

            for (int move : {6, 10, 15, 17, -6, -10, -15, -17}) {
                int to = from + move;
                if (isKnigthMove(from, to)) {
                    Move m{ from, to, KNIGHT, NULL_TYPE };
                    moves.push_back(m);
                }
            }

        }
    }
}

void MoveGenerator::generateTowerMoves(const Board& board, std::vector<Move>& moves, int color, bool forQueen) 
{
    uint64_t tower;

    if (forQueen) {
        if (color == WHITE) {
            tower = board.getBitboardFromType(Board::W_QUEEN);
        }
        else if (color == BLACK) {
            tower = board.getBitboardFromType(Board::B_QUEEN);
        }
    } else {
        if (color == WHITE) {
            tower = board.getBitboardFromType(Board::W_TOWER);
        }
        else if (color == BLACK) {
            tower = board.getBitboardFromType(Board::B_TOWER);
        }
    }
    

    for (int from = 0; from < 64; from++) {
        uint64_t from_mask = 1ULL << from;
        if (from_mask & tower) {
            // Upward moves
            for (int to = from + 8; to < 64; to+=8) {
                Move m{from, to, (forQueen ? QUEEN : TOWER), NULL_TYPE};
                moves.push_back(m);
            }
            // Downward moves
            for (int to = from - 8; to >= 0; to-=8) {
                Move m{ from, to, (forQueen ? QUEEN : TOWER), NULL_TYPE };
                moves.push_back(m);
            }
            // Right moves
            for (int to = from + 1; to < 64; to++) {
                int from_rank = from / 8;
                int to_rank = to / 8;
                if (from_rank == to_rank) {
                    Move m{from, to, (forQueen ? QUEEN : TOWER), NULL_TYPE };
                    moves.push_back(m);
                } else {
                    break;
                }
            }
            // Left moves
            for (int to = from - 1; to >= 0; to--) {
                int from_rank = from / 8;
                int to_rank = to / 8;
                if (from_rank == to_rank) {
                    Move m{ from, to, (forQueen ? QUEEN : TOWER), NULL_TYPE };
                    moves.push_back(m);
                }
                else {
                    break;
                }
            }
        }
    }
}

void MoveGenerator::generateQueenMoves(const Board& board, std::vector<Move>& moves, int color)
{
    generateBishopMoves(board, moves, color, true);
    generateTowerMoves(board, moves, color, true);
}

void MoveGenerator::generateKingMoves(const Board& board, std::vector<Move>& moves, int color)
{
    uint64_t king;
    if (color == WHITE) {
        king = board.getBitboardFromType(Board::W_KING);
    }
    else if (color == BLACK) {
        king = board.getBitboardFromType(Board::B_KING);
    }

    for (int from = 0; from < 64; from++) {
        uint64_t from_mask = 1ULL << from;
        if (from_mask & king) {
            int from_rank = from / 8;
            int from_col = from % 8;

            // Upwards
            if (from_rank <= 6) {
                int to = from + 8;
                Move m{ from, to, KING, NULL_TYPE };
                moves.push_back(m);
            }
            // Downwards
            if (from_rank >= 1) {
                int to = from - 8;
                Move m{ from, to, KING, NULL_TYPE };
                moves.push_back(m);
            }
            // Right
            if (from_col <= 6) {
                int to = from + 1;
                Move m{ from, to, KING, NULL_TYPE };
                moves.push_back(m);
            }
            // Left
            if (from_col >= 1) {
                int to = from - 1;
                Move m{ from, to, KING, NULL_TYPE };
                moves.push_back(m);
            }
            // Right - Up
            if ( from_rank <= 6 && from_col <= 7) {
                int to = from + 9;
                if (isSameDiagonal(from, to)) {
                    Move m{ from, to, KING, NULL_TYPE };
                    moves.push_back(m);
                }
            }
            // Left - Up
            if (from_rank <= 6 && from_col >= 1) {
                int to = from + 7;
                if (isSameDiagonal(from, to)) {
                    Move m{ from, to, KING, NULL_TYPE };
                    moves.push_back(m);
                }
            }
            // Right - Down
            if (from_rank >= 1 && from_col <= 6) {
                int to = from - 7;
                if (isSameDiagonal(from, to)) {
                    Move m{ from, to, KING, NULL_TYPE };
                    moves.push_back(m);
                }
            }
            // Left - Down
            if (from_rank >= 1 && from_col >= 1) {
                int to = from - 9;
                if (isSameDiagonal(from, to)) {
                    Move m{ from, to, KING, NULL_TYPE };
                    moves.push_back(m);
                }
            }
            // Enroque
            if ((color == Board::WHITE && from == 4) ||
                (color == Board::BLACK && from == 60)) {
                Move m1{ from, from + 2, KING, NULL_TYPE };
                Move m2{ from, from - 2, KING, NULL_TYPE };
                moves.push_back(m1);
                moves.push_back(m2);
            }
        }
    }
}

bool MoveGenerator::isHorizontalPathClear(Move m, uint64_t occupied) {
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

bool MoveGenerator::isVerticalPathClear(Move m, uint64_t occupied) {
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

bool isDiagonalPathClear(Move m, uint64_t occupied) {
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

bool MoveGenerator::isKingInCheck(const Board& board, int kingColor) {
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

    uint64_t enemyPawns = board.getBitboardFromType( (oppColor==Board::WHITE) ? Board::W_PAWN : Board::B_PAWN );
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

bool MoveGenerator::isSquareUnderAttack(const Board& board, int square, int attackerColor) {
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
    attacks |= generateKnightAttacks(enemyTower, position, occupied);
    attacks |= generateKnightAttacks(enemyQueen, position, occupied);
    attacks |= generateKnightAttacks(enemyKing, position, occupied);

    if (square & attacks) {
        return true;
    }

    return false;
}

bool MoveGenerator::isLegal(const Board& board, Move m, int color) {
    int oppColor = ((color == WHITE) ? BLACK : WHITE);
    int pieceIndex = m.piece;
    uint64_t occupied = board.getOccupiedBitBoard();
    uint64_t position = (board.isWhiteToMove() ? board.getWhiteBitBoard() : board.getBlackBitBoard());
    uint64_t oppPosition = (board.isWhiteToMove() ? board.getBlackBitBoard() : board.getWhiteBitBoard());
    uint64_t toMask = 1ULL << m.to;
    Move lastMove = board.getLastMove();

    // Verificar si la casilla de destino está ocupada por una pieza del mismo color
    if (toMask & position) {
        return false;
    }

    // Comprobar si el camino está despejado según el tipo de movimiento
    if ((pieceIndex == TOWER || pieceIndex == QUEEN) && isSameRank(m.from, m.to) && !(isHorizontalPathClear(m, occupied))) {
        return false;
    } 
    else if ((pieceIndex == TOWER || pieceIndex == QUEEN) && isSameColumn(m.from, m.to) && !(isVerticalPathClear(m, occupied))) {
        return false;
    } 
    else if ((pieceIndex == BISHOP || pieceIndex == QUEEN) && isSameDiagonal(m.from, m.to) && !(isDiagonalPathClear(m, occupied))) {
        return false;
    }
    else if (pieceIndex == KNIGHT && !(isKnigthMove(m.from, m.to))) { // Comprobación para los caballos
        return false;
    }
    else if (pieceIndex == PAWN) {
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
            else if (lastMove.piece == PAWN && abs(lastMove.from - lastMove.to) == 16) { // Anterior movimiento es de peón, de dos casillas
                int passantSquare = lastMove.to; // Casilla donde se encuentra la pieza contraria
                int passantLandingSqu = lastMove.to + direction; // Casilla a donde debiera de moverse la pieza atacante para capturar en passant
                if (m.to == passantLandingSqu && (abs(passantSquare - m.from) == 1)) { // Casilla adyacente
                    goto CHECK_KING;
                }
            }
        }
        return false;
    }
    else if (pieceIndex == KING) {
        int diff = abs(m.from - m.to);
        if (diff == 1 || diff == 9 || diff == 7 || diff == 8) {
            goto CHECK_KING;
        }
        
        if (((color == WHITE) && (m.from == 4) && board.getWhiteCastle())) {
            if (isSameRank(m.from, m.to) && (diff == 2)) { // El rey se mueve dos a la izq o der
                if (isHorizontalPathClear(m, occupied)) {
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
            if (isSameRank(m.from, m.to) && (diff == 2)) { // El rey se mueve dos a la izq o der
                if (isHorizontalPathClear(m, occupied)) {
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

void MoveGenerator::filterMoves(const Board& board, std::vector<Move>& moves, int color) {
    for (int i = 0; i < moves.size(); i++) {
        Move move = moves.at(i);
        if (!isLegal(board, move, color)) {
            moves.erase(moves.begin()+i);
            i--;
        }
    }
}

uint64_t MoveGenerator::generatePawnAttacks(uint64_t pawns, int color) {
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

uint64_t MoveGenerator::generateBishopAttacks(uint64_t bishops, uint64_t opposition, uint64_t occupied) {
    uint64_t attacks = 0ULL;
    for (int square = 0; square < 64; square++) {
        if ((bishops & (1ULL << square))) {
            // Diagonal superior derecha
            for (int to = square + 9; to < 64; to += 9) {
                uint64_t attack = 1ULL << to;
                // Límites del tablero
                if (((1ULL << square) & Board::right) || ((1ULL << square) & Board::top)) {
                    break;
                }
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

            //Diagonal inferior izquierda
            for (int to = square - 9; to >=0; to -= 9) {
                uint64_t attack = 1ULL << to;
                // Límites del tablero
                if (((1ULL << square) & Board::left) || ((1ULL << square) & Board::bottom)) {
                    break;
                }
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

            //Diagonal superior izquierda
            for (int to = square + 7; to < 64; to+=7) {
                uint64_t attack = 1ULL << to;
                // Límites del tablero
                if (((1ULL << square) & Board::left) || ((1ULL << square) & Board::top)) {
                    break;
                }
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

            // Diagonal inferior derecha
            for (int to = square - 7; to >= 0; to-=7) {
                uint64_t attack = 1ULL << to;
                // Límites del tablero
                if (((1ULL << square) & Board::right) || ((1ULL << square) & Board::bottom)) {
                    break;
                }
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
    return attacks;
}

uint64_t MoveGenerator::generateKnightAttacks(uint64_t knights, uint64_t opposition, uint64_t occupied) {
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

uint64_t MoveGenerator::generateTowerAttacks(uint64_t towers, uint64_t opposition, uint64_t occupied) {
    uint64_t attacks = 0ULL;
    for (int square = 0; square < 64; square++) {
        if ((1ULL << square) & towers) {
            int fromRank = square / 8;
            // Hacia la derecha
            if (!((1ULL << square) & Board::right)) { // Límite derecho del tablero
                for (int to = (square + 1); ((to / 8) == fromRank) && (to<64); to++) {
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

uint64_t MoveGenerator::generateQueenAttacks(uint64_t queens, uint64_t opposition, uint64_t occupied) {
    uint64_t attacks = 0ULL;
    for (int square = 0; square < 64; square++) {
        if (!((1ULL << square) && queens)) {
            attacks |= generateBishopAttacks(queens, opposition, occupied);
            attacks |= generateTowerAttacks(queens, opposition, occupied);
        }
    }
    return attacks;
}

uint64_t MoveGenerator::generateKingAttacks(uint64_t king, uint64_t opposition, uint64_t occupied) {
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