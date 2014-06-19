#include <UnitTest++.h>
#include <MorseDataSource.h>
#include <MorseDictionary.h>
#include <iostream>

using namespace std;
/*
TEST(DataSource_breakIntoElements) {
    MorseDataSource dataSource("a", MorseDictionary::defaultDictionary());

    CHECK(!dataSource.finished());

    CHECK(dataSource.get() == MorseElement::Dot);
    CHECK(dataSource.get() == MorseElement::SilentDot);
    CHECK(dataSource.get() == MorseElement::Dash);

    CHECK(dataSource.finished());

    CHECK(dataSource.get() == MorseElement::None);
}

TEST(DataSource_intracharSpace) {
    MorseDataSource dataSource("ee", MorseDictionary::defaultDictionary());

    CHECK(dataSource.get() == MorseElement::Dot);
    CHECK(dataSource.get() == MorseElement::SpaceBetweenChars);
    CHECK(dataSource.get() == MorseElement::Dot);

    CHECK(dataSource.finished());
}

TEST(DataSource_intrawordSpace) {
    MorseDataSource dataSource("a eee", MorseDictionary::defaultDictionary());

    CHECK(dataSource.get() == MorseElement::Dot);
    CHECK(dataSource.get() == MorseElement::SilentDot);
    CHECK(dataSource.get() == MorseElement::Dash);
    CHECK(dataSource.get() == MorseElement::SpaceBetweenWords);
    CHECK(dataSource.get() == MorseElement::Dot);
    CHECK(dataSource.get() == MorseElement::SpaceBetweenChars);
    CHECK(dataSource.get() == MorseElement::Dot);
    CHECK(dataSource.get() == MorseElement::SpaceBetweenChars);
    CHECK(dataSource.get() == MorseElement::Dot);

    CHECK(dataSource.finished());
}
*/