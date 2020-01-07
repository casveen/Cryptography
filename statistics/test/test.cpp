#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "statistics.hpp"

template<class T>
bool vector_contains(vector<T> looking_in, T looking_for) {
    bool found_one= false;
    for (T element : looking_in) {
        if (element == looking_for) {
            if (found_one) { cout << "duplicate!\n"; }
            found_one= true;
        };
    }
    return found_one;
}

SCENARIO("Choices of sets", "[choices]") {
    GIVEN("A set {A, B, C}") {
        vector<char> set({'A', 'B', 'C'});
        WHEN("Choosing 3 elements") {
            vector<vector<char>> choices= choices_of<char>(set, 3);
            THEN("Should return 3!=6 distinct sets") {
                REQUIRE(choices.size() == 6);
                REQUIRE(vector_contains(choices, {'A', 'B', 'C'}));
                REQUIRE(vector_contains(choices, {'A', 'C', 'B'}));
                REQUIRE(vector_contains(choices, {'B', 'A', 'C'}));
                REQUIRE(vector_contains(choices, {'C', 'A', 'B'}));
                REQUIRE(vector_contains(choices, {'B', 'C', 'A'}));
                REQUIRE(vector_contains(choices, {'C', 'B', 'A'}));
            }
        }
        WHEN("Choosing 2 elements") {
            vector<vector<char>> choices= choices_of<char>(set, 2);
            THEN("Should return 3!/1!=6 distinct sets") {
                REQUIRE(choices.size() == 6);
                REQUIRE(vector_contains(choices, {'A', 'B'}));
                REQUIRE(vector_contains(choices, {'A', 'C'}));
                REQUIRE(vector_contains(choices, {'B', 'A'}));
                REQUIRE(vector_contains(choices, {'B', 'C'}));
                REQUIRE(vector_contains(choices, {'C', 'A'}));
                REQUIRE(vector_contains(choices, {'C', 'B'}));
            }
        }
    }
    GIVEN("A set {A, B, C, D}") {
        vector<char> set({'A', 'B', 'C', 'D'});
        WHEN("Choosing 3 elements") {
            vector<vector<char>> choices= choices_of<char>(set, 3);
            THEN("Should return 4!/1!=24 distinct sets") {
                REQUIRE(choices.size() == 24);
                REQUIRE(vector_contains(choices, {'A', 'B', 'C'}));
                REQUIRE(vector_contains(choices, {'A', 'B', 'D'}));
                REQUIRE(vector_contains(choices, {'A', 'C', 'B'}));
                REQUIRE(vector_contains(choices, {'A', 'C', 'D'}));
                REQUIRE(vector_contains(choices, {'A', 'D', 'B'}));
                REQUIRE(vector_contains(choices, {'A', 'D', 'C'}));

                REQUIRE(vector_contains(choices, {'B', 'A', 'C'}));
                REQUIRE(vector_contains(choices, {'B', 'A', 'D'}));
                REQUIRE(vector_contains(choices, {'B', 'C', 'A'}));
                REQUIRE(vector_contains(choices, {'B', 'C', 'D'}));
                REQUIRE(vector_contains(choices, {'B', 'D', 'A'}));
                REQUIRE(vector_contains(choices, {'B', 'D', 'C'}));

                REQUIRE(vector_contains(choices, {'C', 'B', 'A'}));
                REQUIRE(vector_contains(choices, {'C', 'B', 'D'}));
                REQUIRE(vector_contains(choices, {'C', 'A', 'B'}));
                REQUIRE(vector_contains(choices, {'C', 'A', 'D'}));
                REQUIRE(vector_contains(choices, {'C', 'D', 'B'}));
                REQUIRE(vector_contains(choices, {'C', 'D', 'A'}));

                REQUIRE(vector_contains(choices, {'D', 'B', 'C'}));
                REQUIRE(vector_contains(choices, {'D', 'B', 'A'}));
                REQUIRE(vector_contains(choices, {'D', 'C', 'B'}));
                REQUIRE(vector_contains(choices, {'D', 'C', 'A'}));
                REQUIRE(vector_contains(choices, {'D', 'A', 'B'}));
                REQUIRE(vector_contains(choices, {'D', 'A', 'C'}));
            }
        }
        WHEN("Choosing 2 elements") {
            vector<vector<char>> choices= choices_of<char>(set, 2);
            THEN("Should return 4!/2!=12 distinct sets") {
                REQUIRE(choices.size() == 12);
                REQUIRE(vector_contains(choices, {'A', 'B'}));
                REQUIRE(vector_contains(choices, {'A', 'C'}));
                REQUIRE(vector_contains(choices, {'A', 'D'}));

                REQUIRE(vector_contains(choices, {'B', 'A'}));
                REQUIRE(vector_contains(choices, {'B', 'C'}));
                REQUIRE(vector_contains(choices, {'B', 'D'}));

                REQUIRE(vector_contains(choices, {'C', 'B'}));
                REQUIRE(vector_contains(choices, {'C', 'A'}));
                REQUIRE(vector_contains(choices, {'C', 'D'}));

                REQUIRE(vector_contains(choices, {'D', 'B'}));
                REQUIRE(vector_contains(choices, {'D', 'C'}));
                REQUIRE(vector_contains(choices, {'D', 'A'}));
            }
        }
        WHEN("Choosing 1 element") {
            vector<vector<char>> choices= choices_of<char>(set, 1);
            THEN("Should return 4!/3!=4 distinct sets") {
                REQUIRE(choices.size() == 4);
                REQUIRE(vector_contains(choices, {'A'}));
                REQUIRE(vector_contains(choices, {'B'}));
                REQUIRE(vector_contains(choices, {'C'}));
                REQUIRE(vector_contains(choices, {'D'}));
            }
        }
        WHEN("Choosing 0 elements") {
            vector<vector<char>> choices= choices_of<char>(set, 0);
            THEN("Should return 1 empty element") {
                REQUIRE(choices.size() == 1);
                REQUIRE(choices[0].size() == 0);
            }
        }
    }
}
