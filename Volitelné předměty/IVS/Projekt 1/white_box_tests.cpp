//======== Copyright (c) 2022, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - test suite
//
// $NoKeywords: $ivs_project_1 $white_box_tests.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2023-03-07
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace testu hasovaci tabulky.
 */

#include <vector>

#include "gtest/gtest.h"

#include "white_box_code.h"

namespace WhiteBoxTesting
{

	using namespace ::testing;

    class NonEmptyTable : public Test
    {
        void SetUp()
        {

        }
    protected:
        hash_map_t *hashmap;
    };
    class EmptyTable : public Test
    {
        void SetUp()
        {
            hashmap = hash_map_ctor();
        }
    protected:
        hash_map_t *hashmap;
    };



TEST_F(EmptyTable, ReturnsValidPointer)
{
    ASSERT_NE(hashmap, nullptr);
}

TEST_F(EmptyTable, FirstIsNull)
{
    ASSERT_EQ(hashmap->first, nullptr);
}

TEST_F(EmptyTable, LastIsNull)
{
    ASSERT_EQ(hashmap->last, nullptr);
}

TEST_F(EmptyTable, IndextIsNull)
{
    ASSERT_EQ(hashmap->index, nullptr);
}

TEST_F(EmptyTable, DummyIsNull)
{
    ASSERT_EQ(hashmap->dummy, nullptr);
}

TEST_F(EmptyTable, Ctor_Equal)
{
    ASSERT_EQ(hashmap->allocated, HASH_MAP_INIT_SIZE);
}

TEST_F(EmptyTable, UsedZero)
{
    ASSERT_EQ(hashmap->used, 0);

}
TEST_F(EmptyTable, Ctor_InedxIsAllocated)
{
    ASSERT_NE(hashmap->index, nullptr);
}

TEST_F(EmptyTable, Ctor_IndesIsProperlyAllocated)
{
    size_t expected_size = HASH_MAP_INIT_SIZE * sizeof(hash_map_item_t*);
    size_t actual_size = hashmap->allocated * sizeof(hash_map_item_t*);
    ASSERT_EQ(actual_size, expected_size);
}
TEST_F(NonEmptyTable, ConstructsHashMap)
{
    hash_map_t* map = hash_map_ctor();
    ASSERT_NE(nullptr, map);
    EXPECT_EQ(HASH_MAP_INIT_SIZE, map->allocated);
    EXPECT_EQ(0, map->used);
    for(size_t i = 0; i < map->allocated; ++i)
    {
        EXPECT_EQ(nullptr, map->index[i]);
    }
    EXPECT_EQ(nullptr, map->first);
    EXPECT_EQ(nullptr, map->last);
    hash_map_dtor(map);
}

TEST_F(EmptyTable, ClearsEmptyHashMap) 
{
    hash_map_clear(hashmap);
    EXPECT_EQ(0, hashmap->used);
    EXPECT_EQ(nullptr, hashmap->first);
    EXPECT_EQ(nullptr, hashmap->last);
    EXPECT_EQ(nullptr, hashmap->index[0]);
}

TEST_F(EmptyTable, SizeReturnsZero)
{
    size_t expected_size = 0;

    size_t actual_size = hash_map_size(hashmap);

    EXPECT_EQ(expected_size, actual_size);
}
TEST_F(EmptyTable, HashMapCapacityTest)
{
    hash_map_t* map = hash_map_ctor();
    EXPECT_EQ(8, hash_map_capacity(map));
    hash_map_dtor(map);
}
TEST_F(EmptyTable, EmptyMapWithZeroSize)
{
    hash_map_t* map = hash_map_ctor();
    ASSERT_NE(nullptr, map);

    EXPECT_EQ(OK, hash_map_reserve(map, 0));

    EXPECT_EQ(0, map->allocated);
    EXPECT_EQ(0, map->used);

    hash_map_dtor(map);
}

TEST_F(EmptyTable, EmptyMapWithNonZeroSize)
{
    hash_map_t* map = hash_map_ctor();
    ASSERT_NE(nullptr, map);

    EXPECT_EQ(OK, hash_map_reserve(map, 4));

    EXPECT_EQ(4, map->allocated);
    EXPECT_EQ(0, map->used);

    for(size_t i = 0; i < map->allocated; ++i)
    {
        EXPECT_EQ(nullptr, map->index[i]);
    }
    hash_map_dtor(map);
}
TEST_F(EmptyTable, EmptyMapWithLowersSizeThanInit)
{
    hash_map_t* map = hash_map_ctor();
    ASSERT_NE(nullptr, map);

    EXPECT_EQ(OK, hash_map_reserve(map, 8));

    EXPECT_EQ(HASH_MAP_INIT_SIZE, map->allocated);
    EXPECT_EQ(0, map->used);

    for (size_t i = 0; i < map->allocated; ++i)
    {
        EXPECT_EQ(nullptr, map->index[i]);
    }

    hash_map_dtor(map);
}
TEST_F(EmptyTable, EmptyMapWithGreaterSizeThanInit)
{
    hash_map_t* map = hash_map_ctor();
    ASSERT_NE(nullptr, map);

    EXPECT_EQ(OK, hash_map_reserve(map, 16));

    EXPECT_EQ(16, map->allocated);
    EXPECT_EQ(0, map->used);

    for(size_t i = 0; i < map->allocated; ++i)
    {
        EXPECT_EQ(nullptr, map->index[i]);
    }
    hash_map_dtor(map);
}
TEST_F(EmptyTable, ContainsReturnsFalse)
{
    hash_map_t* map = hash_map_ctor();
    ASSERT_NE(nullptr, map);
    EXPECT_FALSE(hash_map_contains(map, "key1"));
    EXPECT_FALSE(hash_map_contains(map, "key2"));
    EXPECT_FALSE(hash_map_contains(map, "key3"));
    hash_map_dtor(map);
}

TEST_F(EmptyTable, EmptyPut)
{
    hash_map_t* map = hash_map_ctor();
    hash_map_state_code_t result = hash_map_put(map, "key1", 1);
    ASSERT_EQ(OK, result);
    ASSERT_EQ(1, hash_map_size(map));
    ASSERT_EQ(8, hash_map_capacity(map));
    ASSERT_TRUE(hash_map_contains(map, "key1"));

    result = hash_map_put(map, "key2", 2);
    ASSERT_EQ(OK, result);
    ASSERT_EQ(2, hash_map_size(map));
    ASSERT_EQ(8, hash_map_capacity(map));
    ASSERT_TRUE(hash_map_contains(map, "key2"));

    hash_map_dtor(map);
}
TEST_F(EmptyTable, GetExistentKey)
{

    hash_map_t* map = hash_map_ctor();

    hash_map_put(map, "key1", 1);
    hash_map_put(map, "key2", 2);
    hash_map_put(map, "key3", 3);

    int value;
    hash_map_state_code_t result = hash_map_get(map, "key2", &value);

    EXPECT_EQ(result, OK);
    EXPECT_EQ(value, 2);

    hash_map_dtor(map);
}
TEST_F(EmptyTable, GetNonExistentKey)
{
    hash_map_t* map = hash_map_ctor();

    hash_map_put(map, "key1", 1);
    hash_map_put(map, "key2", 2);
    hash_map_put(map, "key3", 3);

    int value;
    hash_map_state_code_t result = hash_map_get(map, "key4", &value);

    EXPECT_EQ(result, KEY_ERROR);

    hash_map_dtor(map);
}
TEST_F(EmptyTable, RemoveExistingKey)
{
    hash_map_t* map = hash_map_ctor();
    hash_map_put(map, "key1", 1);
    hash_map_put(map, "key2", 2);
    hash_map_put(map, "key3", 3);

    hash_map_state_code_t result = hash_map_remove(map, "key2");
    EXPECT_EQ(OK, result);

    int value;
    result = hash_map_get(map, "key2", &value);
    EXPECT_EQ(KEY_ERROR, result);

    hash_map_dtor(map);


}
}


//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy hasovaci tabulky, testujte nasledujici:
// 1. Verejne rozhrani hasovaci tabulky
//     - Vsechny funkce z white_box_code.h
//     - Chovani techto metod testuje pro prazdnou i neprazdnou tabulku.
// 2. Chovani tabulky v hranicnich pripadech
//     - Otestujte chovani pri kolizich ruznych klicu se stejnym hashem 
//     - Otestujte chovani pri kolizich hashu namapovane na stejne misto v 
//       indexu
//============================================================================//

/*** Konec souboru white_box_tests.cpp ***/
