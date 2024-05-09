#include "net.hpp"

using namespace aq;

Net::LocalizedIterator::LocalizedIterator(Net &net, const Fish &centerFish) : net(net), centerCord(net.getCord(centerFish)) {
    updateIters();
    while (currIter == currEnd) {
        idx++;
        if (idx >= 9) return;
        updateIters();
    }
}

Net::LocalizedIterator &Net::LocalizedIterator::operator++() {
    currIter++;
    while (currIter == currEnd) {
        idx++;
        if (idx >= 9) return *this;
        updateIters();
    }
    return *this;
}
