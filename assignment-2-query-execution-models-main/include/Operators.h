/**
 * @file
 *
 * Base operator methods and initialization of static variables.
 *
 * Based on code from Henning Funke (TU-Dortmund)
 *
 * @authors: Jana Giceva <jana.giceva@in.tum.de>, Alexander Beischl
 * <beischl@in.tum.de>
 */

/* TODO: Add vector-at-a-time functionality for each operators */

#pragma once

#include <pthread.h>

#include <cstdlib>
#include <iostream>
#include <thread>

#include "BaseOperator.h"
#include "DBData.h"

/**
 * @brief Methods to drive the plan execution for pull-based execution models.
 */
class PullDriver {
  public:
    /**
     * @brief Execute query plan with Volcano (Tuple-at-a-time) and write
     * result.
     */
    static void volcano(RelOperator* node, Relation* result) {
        node->open();
        Tuple* t = node->next();
        size_t outLen = 0;
        Tuple* r = result->r;
        while (t != nullptr) {
            r[outLen++] = *t;
            t = node->next();
        }
        result->len = outLen;
        node->close();
    }

    static void vector(RelOperator* node, Relation* result) {
        // TODO: Execute query plan with Vector-at-a-time and write
        // result.
        node->openVec();
        Relation* t = node->nextVec();
        size_t outLen = 0;
        Tuple* r = result->r;
        while (t != nullptr) {
            for (size_t i = 0; i < t->len; i++) {
                r[outLen++] = t->r[i];
            }
            t = node->nextVec();
        }
        result->len = outLen;
        node->closeVec();
    }
};

/**
 * @brief Operator for scanning a relation.
 */
class ScanOp : public RelOperator {
  protected:
    Tuple* table;
    size_t tableSize;
    /* volcano (and vector-at-a-time) */
    size_t cursor;
    /* operator-at-a-time */
    Relation oCol;

    /* vector-at-a-time */
    size_t block_size = 8192;
    Relation block;

  public:
    ScanOp(Tuple* tab, size_t n) : RelOperator(nullptr) {
        this->table = tab;
        this->tableSize = n;
        this->oCol = allocateRelation(n);
    }

    virtual ~ScanOp() { freeRelation(this->oCol); }

    virtual size_t getSize() { return tableSize; }

    virtual void open();
    virtual Tuple* next();
    virtual void close();

    virtual void openVec();
    virtual Relation* nextVec();
    virtual void closeVec();

    virtual Relation getRelation();
};

/**
 * @brief Operator for selection.
 */
class SelectionOp : public RelOperator {
  public:
    enum PredicateType { SMALLER, EQUALS, EQUALS_NOT };

  protected:
    SelectionOp::PredicateType type;
    int compareConstant;

  public:
    SelectionOp(PredicateType type, int compareConstant, RelOperator* child)
        : RelOperator(child) {
        this->type = type;
        this->compareConstant = compareConstant;
    }

    virtual ~SelectionOp(){};

    virtual size_t getSize() { return child->getSize(); }

    virtual void open();
    virtual Tuple* next();
    virtual void close();

    virtual void openVec();
    virtual Relation* nextVec();
    virtual void closeVec();

    virtual Relation getRelation();
};

/**
 * @brief Aggregation operator.
 */
class AggregationOp : public RelOperator {
  public:
    enum ReduceType { COUNT, SUM };

  protected:
    AggregationOp::ReduceType type;
    Tuple countTuple;
    bool hasMoreTuples;

    /* operator-at-a-time */
    Relation oCol;

    /* vector-at-a-time */
    Relation block;

  public:
    AggregationOp(ReduceType type, RelOperator* child) : RelOperator(child) {
        this->oCol = allocateRelation(1);
        this->block = allocateRelation(1);
        this->type = type;
    }

    virtual ~AggregationOp() {
        freeRelation(this->oCol);
        freeRelation(this->block);
    }

    virtual size_t getSize() { return 1; }

    virtual void open();
    virtual Tuple* next();
    virtual void close();

    virtual void openVec();
    virtual Relation* nextVec();
    virtual void closeVec();

    virtual Relation getRelation();
};
