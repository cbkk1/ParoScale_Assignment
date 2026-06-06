#include "index.hpp"
#include "protocol.hpp"
#include <cassert>
#include <cstdio>

int main() {
    Index idx;
    assert(idx.index("a", {}));              
    assert(idx.query("a"));
    assert(!idx.index("b", {"x"}));             
    idx.index("b", {"a"});
    assert(!idx.remove("a"));              
    assert(idx.remove("b"));
    assert(idx.remove("a"));                    
    assert(idx.remove("ghost"));                

    std::string c, p; std::vector<std::string> d;
    assert(parseLine("INDEX|cloog|gmp,isl", c, p, d));
    assert(!parseLine("INDEX|emacs=elisp", c, p, d));
    assert(!parseLine("LIZARD|a|b", c, p, d));

    std::printf("all tests passed\n");
    return 0;
}
