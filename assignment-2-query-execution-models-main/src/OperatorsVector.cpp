/**
 * @file
 *
 * Implementation of vector-at-a-time execution for relational operators.
 *
 */

/* TODO: Add openVec(), nextVec() and closeVec() implementations for vector-at-a-time processing */

#include "Operators.h"
#include "primitives.h"

#include <iostream>

void ScanOp::openVec() {
    this->cursor = 0;
    this->block = allocateRelation(this->block_size);
}

Relation* ScanOp::nextVec() {
    if(cursor >= tableSize) {
        return nullptr;
    }
    if(cursor+block_size >= tableSize) {
        block.len = scanLong ( table+cursor, block.r, this->tableSize-cursor );
        cursor += block.len;
        return &block;
    }
    block.len = scanLong ( table+cursor, block.r, this->block_size );
    cursor += block.len; 
    return &block; 
}

void ScanOp::closeVec() {
    freeRelation(this->block);
}

void SelectionOp::openVec() {
    child->openVec();
}

Relation* SelectionOp::nextVec() {
    while (true)
    {
        Relation* in = child->nextVec();
        if(in == nullptr) return nullptr;
        switch ( this->type ) {
            case PredicateType::EQUALS:
                in->len = compareEquals ( in->r, this->compareConstant, in->r, in->len );
                return in;
                break;
            case PredicateType::EQUALS_NOT:
                in->len = compareNotEquals ( in->r, this->compareConstant, in->r, in->len );
                return in;
                break;
            case PredicateType::SMALLER:
                in->len = compareSmaller ( in->r, this->compareConstant, in->r, in->len );
                return in;
                break;
        }
    }
    
    return nullptr;
    
}

void SelectionOp::closeVec() {
    child->closeVec();
}

void AggregationOp::openVec() {
    child->openVec();
    countTuple = 0;
    this->hasMoreTuples = true;
}

Relation* AggregationOp::nextVec() {
    if ( !this->hasMoreTuples ) {
        return nullptr;
    }
    block.len = 1;
    while(true) {  
        Relation* in = child->nextVec();
        if(in == nullptr) break;
        if ( this->type == AggregationOp::ReduceType::COUNT ) {
            this->countTuple += aggCount ( in->r, in->len );
        }
        if ( this->type == AggregationOp::ReduceType::SUM ) {
            this->countTuple += aggSum ( in->r, in->len );
        }
    }
    block.r[0] = this->countTuple;
    this->hasMoreTuples = false;
    return &block;
}

void AggregationOp::closeVec() {
    child->close();
}