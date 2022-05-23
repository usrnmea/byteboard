#include "unity.h"
#include "piece.h"

void test_type_of_piece(void)
{
    TEST_ASSERT_EQUAL_UINT32(PAWN, type_of_piece(B_PAWN));
    TEST_ASSERT_EQUAL_UINT32(KNIGHT, type_of_piece(W_KNIGHT));
    TEST_ASSERT_EQUAL_UINT32(QUEEN, type_of_piece(W_QUEEN));
}

void test_color_of_piece(void)
{
    TEST_ASSERT_EQUAL_UINT32(BLACK, color_of_piece(B_PAWN));
    TEST_ASSERT_EQUAL_UINT32(WHITE, color_of_piece(W_KNIGHT));
    TEST_ASSERT_EQUAL_UINT32(WHITE, color_of_piece(W_QUEEN));
}

void test_make_piece(void)
{
    TEST_ASSERT_EQUAL_UINT32(B_PAWN, make_piece(BLACK, PAWN));
    TEST_ASSERT_EQUAL_UINT32(W_KNIGHT, make_piece(WHITE, KNIGHT));
    TEST_ASSERT_EQUAL_UINT32(B_BISHOP, make_piece(BLACK, BISHOP));
}