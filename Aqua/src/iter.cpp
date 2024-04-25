#include "net.hpp"

using namespace aq;

Net::LocalisedIterator::LocalisedIterator(Net &net, const Fish &centerFish) : net(net), centerCord(net.getCord(centerFish)) {
    updateIters();
    while (currIter == currEnd) {
        idx++;
        updateIters();
    }
}

Net::LocalisedIterator &Net::LocalisedIterator::operator++() {
    currIter++;
    while (currIter == currEnd) {
        idx++;
        if (idx == 9) return *this;
        updateIters();
    }
    return *this;
}
