#include "game/logic/magic_bitboards.hpp"

#include <cstdint>
#include <random>

// Magic Bitboards
    // Magic number generation taken from https://www.chessprogramming.org/Looking_for_Magics
    // Magic bitboard logic taken from https://www.chessprogramming.org/Magic_Bitboards#Fancy
namespace {
    using u64 = uint64_t;

    std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<u64> dist64;

    u64 random_uint64() {
        return dist64(rng);;
    }

    u64 random_uint64_fewbits() {
        return random_uint64() & random_uint64() & random_uint64();
    }

    int count_1s(const u64 bb) {
        return __builtin_popcountll(bb);
    }
    //int count_1s(u64 b) {
    //    int r;
    //    for(r = 0; b; r++, b &= b - 1);
    //    return r;
    //}

    constexpr int BitTable[64] = {
        63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
        51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
        26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
        58, 20, 37, 17, 36, 8
    };

    int pop_1st_bit(u64& bb) {
        const u64 b = bb ^ (bb - 1);
        const unsigned int fold = static_cast<unsigned>(((b & 0xffffffff) ^ (b >> 32)));
        bb &= (bb - 1);
        return BitTable[(fold * 0x783a9b23) >> 26];
    }

    u64 index_to_uint64(const int index, const int bits, u64 m) {
        int i, j;
        u64 result = 0ULL;
        for(i = 0; i < bits; i++) {
            j = pop_1st_bit(m);
            if(index & (1 << i)) result |= (1ULL << j);
        }
        return result;
    }

    u64 rookmask(const int square) {
        u64 result = 0ULL;
        const int rank = square/8, file = square%8;
        int r, f;
        for(r = rank+1; r <= 6; r++) {result |= (1ULL << (file + r*8));}
        for(r = rank-1; r >= 1; r--) {result |= (1ULL << (file + r*8));}
        for(f = file+1; f <= 6; f++) {result |= (1ULL << (f + rank*8));}
        for(f = file-1; f >= 1; f--) {result |= (1ULL << (f + rank*8));}
        return result;
    }

    u64 bishopmask(const int square) {
        u64 result = 0ULL;
        const int rank = square/8, file = square%8;
        int r, f;
        for(r=rank+1, f=file+1; r<=6 && f<=6; r++, f++) {result |= (1ULL << (f + r*8));}
        for(r=rank+1, f=file-1; r<=6 && f>=1; r++, f--) {result |= (1ULL << (f + r*8));}
        for(r=rank-1, f=file+1; r>=1 && f<=6; r--, f++) {result |= (1ULL << (f + r*8));}
        for(r=rank-1, f=file-1; r>=1 && f>=1; r--, f--) {result |= (1ULL << (f + r*8));}
        return result;
    }

    u64 rookattack(int square, u64 block) {
        u64 result = 0ULL;
        int rank = square/8, file = square%8, r, f;
        for(r = rank+1; r <= 7; r++) {
            result |= (1ULL << (file + r*8));
            if(block & (1ULL << (file + r*8))) break;
        }
        for(r = rank-1; r >= 0; r--) {
            result |= (1ULL << (file + r*8));
            if(block & (1ULL << (file + r*8))) break;
        }
        for(f = file+1; f <= 7; f++) {
            result |= (1ULL << (f + rank*8));
            if(block & (1ULL << (f + rank*8))) break;
        }
        for(f = file-1; f >= 0; f--) {
            result |= (1ULL << (f + rank*8));
            if(block & (1ULL << (f + rank*8))) break;
        }
        return result;
    }

    u64 batt(int sq, u64 block) {
        u64 result = 0ULL;
        int rk = sq/8, fl = sq%8, r, f;
        for(r = rk+1, f = fl+1; r <= 7 && f <= 7; r++, f++) {
            result |= (1ULL << (f + r*8));
            if(block & (1ULL << (f + r * 8))) break;
        }
        for(r = rk+1, f = fl-1; r <= 7 && f >= 0; r++, f--) {
            result |= (1ULL << (f + r*8));
            if(block & (1ULL << (f + r * 8))) break;
        }
        for(r = rk-1, f = fl+1; r >= 0 && f <= 7; r--, f++) {
            result |= (1ULL << (f + r*8));
            if(block & (1ULL << (f + r * 8))) break;
        }
        for(r = rk-1, f = fl-1; r >= 0 && f >= 0; r--, f--) {
            result |= (1ULL << (f + r*8));
            if(block & (1ULL << (f + r * 8))) break;
        }
        return result;
    }


    int transform(u64 b, u64 magic, int bits) {
#if defined(USE_32_BIT_MULTIPLICATIONS)
        return
            (unsigned)((int)b*(int)magic ^ (int)(b>>32)*(int)(magic>>32)) >> (32-bits);
#else
        return (int)((b * magic) >> (64 - bits));
#endif
    }

    u64 find_magic(int sq, int m, int bishop) {
        u64 mask, b[4096], a[4096], used[4096], magic;
        int i, j, k, n, fail;

        mask = bishop? bishopmask(sq) : bishopmask(sq);
        n = count_1s(mask);

        for(i = 0; i < (1 << n); i++) {
            b[i] = index_to_uint64(i, n, mask);
            a[i] = bishop? batt(sq, b[i]) : rookattack(sq, b[i]);
        }
        for(k = 0; k < 100000000; k++) {
            magic = random_uint64_fewbits();
            if(count_1s((mask * magic) & 0xFF00000000000000ULL) < 6) continue;
            for(i = 0; i < 4096; i++) used[i] = 0ULL;
            for(i = 0, fail = 0; !fail && i < (1 << n); i++) {
                j = transform(b[i], magic, m);
                if(used[j] == 0ULL) used[j] = a[i];
                else if(used[j] != a[i]) fail = 1;
            }
            if(!fail) return magic;
        }
        printf("***Failed***\n");
        return 0ULL;
    }

    int RBits[64] = {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12
    };

    int BBits[64] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6
    };

    int main() {
        int square;

        printf("const u64 RMagic[64] = {\n");
        for(square = 0; square < 64; square++)
            printf("  0x%llxULL,\n", find_magic(square, RBits[square], 0));
        printf("};\n\n");

        printf("const u64 BMagic[64] = {\n");
        for(square = 0; square < 64; square++)
            printf("  0x%llxULL,\n", find_magic(square, BBits[square], 1));
        printf("};\n\n");

        return 0;
    }
}
