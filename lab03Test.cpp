#include "WordCount.h"
#include "tddFuncs.h"
#include <iostream>

using namespace std;

int main() {
    WordCount wc;
    
    ASSERT_EQUALS(0, wc.getTotalWords());
    ASSERT_EQUALS(0, wc.getNumUniqueWords());
    
    ASSERT_EQUALS(1, wc.incrWordCount("hello"));
    ASSERT_EQUALS(1, wc.getTotalWords());
    ASSERT_EQUALS(1, wc.getNumUniqueWords());
    
    ASSERT_EQUALS(2, wc.incrWordCount("hello"));
    ASSERT_EQUALS(2, wc.getTotalWords());
    
    ASSERT_EQUALS(3, wc.incrWordCount("HELLO"));
    ASSERT_EQUALS(3, wc.getWordCount("hello"));
    
    ASSERT_EQUALS(1, wc.incrWordCount("world"));
    ASSERT_EQUALS(2, wc.getNumUniqueWords());
    
    ASSERT_EQUALS(2, wc.decrWordCount("hello"));
    ASSERT_EQUALS(0, wc.decrWordCount("world"));
    ASSERT_EQUALS(1, wc.getNumUniqueWords());
    
    ASSERT_EQUALS(-1, wc.decrWordCount("nothere"));
    ASSERT_EQUALS(0, wc.incrWordCount(""));
    
    ASSERT_EQUALS(string("can't"), WordCount::makeValidWord("can't"));
    ASSERT_EQUALS(string("monkeys"), WordCount::makeValidWord("12mOnkEYs-$"));
    ASSERT_EQUALS(string("paive"), WordCount::makeValidWord("Pa55ive"));
    
    cout << "All tests passed!" << endl;
    return 0;
}
