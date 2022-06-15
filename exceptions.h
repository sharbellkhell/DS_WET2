#ifndef AVLTREE_EXCEPTIONS_H
#define AVLTREE_EXCEPTIONS_H
#include <exception>
#include <iostream>

class NodeAlreadyExists : public std::exception{
};
class NodeDoesntExist : public std::exception{
};
class InvalidInput : public std::exception{
};

#endif //AVLTREE_EXCEPTIONS_H
