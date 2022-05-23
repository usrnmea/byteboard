#include "unity.h"
#include "bitboard.h"

void test_bit_scan_forward(void)
{
	TEST_ASSERT_EQUAL_UINT32(1, bit_scan_forward(UNIVERSE));
	TEST_ASSERT_EQUAL_UINT32(3, bit_scan_forward(0x10ull));
	TEST_ASSERT_EQUAL_UINT32(63, bit_scan_forward(0x0100000000000000ull));
}

void test_bit_scan_reverse(void)
{
	TEST_ASSERT_EQUAL_UINT32(63, bit_scan_reverse(UNIVERSE));
	TEST_ASSERT_EQUAL_UINT32(3, bit_scan_reverse(0x10ull));
	TEST_ASSERT_EQUAL_UINT32(61, bit_scan_reverse(0x0100000000000000ull));
}
