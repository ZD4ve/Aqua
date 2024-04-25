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
    if (atEnd()) throw std::out_of_range("Iter already at end!");
    currIter++;
    while (currIter == currEnd) {
        idx++;
        if (atEnd()) return *this;
        updateIters();
    }
    return *this;
}
