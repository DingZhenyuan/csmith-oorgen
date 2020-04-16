#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>

#include "DefaultRndNumGenerator.h"
#include "Filter.h"
#include "SequenceFactory.h"
#include "Sequence.h"
#include "CGOptions.h"
#include "DeltaMonitor.h"

DefaultRndNumGenerator *DefaultRndNumGenerator::impl_ = 0;

DefaultRndNumGenerator::DefaultRndNumGenerator(const unsigned long, Sequence *concrete_seq) : rand_depth_(0), trace_string_(""), seq_(concrete_seq) {}
DefaultRndNumGenerator::~DefaultRndNumGenerator() {
    SequenceFactory::destroy_sequences();
}

DefaultRndNumGenerator* DefaultRndNumGenerator::make_rndnum_generator(const unsigned long seed) {
    if (impl_) return impl_;
    Sequence *seq = SequenceFactory::make_sequence();
    
    impl_ = new DefaultRndNumGenerator(seed, seq);
    assert(impl_);
    //impl_->seedrand(seed);

    return impl_;
}

void DefaultRndNumGenerator::get_sequence(std::string &sequence) {
    std::ostringstream ss;
    seq_->get_sequence(ss);
    sequence = ss.str();
}

std::string DefaultRndNumGenerator::get_prefixed_name(const std::string &name) {
    return name;
}

void DefaultRndNumGenerator::add_number(int v, int bound, int k) {
    if (DeltaMonitor::is_running()) {
        seq_->add_number(v, bound, k);
    }
}

unsigned int DefaultRndNumGenerator::rnd_upto(const unsigned int n, const Filter *f, const std::string *where) {
    static int g = 0;
    int h = g;
    if (h == 440) BREAK_NOP; // 用于debug
    unsigned int v = genrand() % n;
    unsigned INT64 local_depth = rand_depth_;
    rand_depth_++;
    if (f) {
        // 不断尝试生成可以通过filter的随机数v
        while (f->filter(v)) {
            //我们可以在前一个过滤器内将数字添加到序列中。
            //如果先前的过滤器失败，则需要在此处回滚rand_depth_。
            //这也会覆盖添加到map中的值。
            rand_depth_ = local_depth+1;
            v = genrand() % n;
        }
    }
    if (where) {
        std::ostringstream ss;
        ss << *where << "->";
        trace_string_ += ss.str();
    }
    add_number(v, n, local_depth);
    return v;
}

bool DefaultRndNumGenerator::rnd_flipcoin(const unsigned int p, const Filter *f, const std::string *) {
    assert(p <= 100);
    unsigned INT64 local_depth = rand_depth_;
    rand_depth_++;
    if (f) {
        if (f->filter(0)) {
            add_number(1, 2, local_depth);
            return true;
        } else if (f->filter(1)) {
            add_number(0, 2, local_depth);
            return false;
        }
    }
    bool rv = (genrand() % 100) < p;
    if (rv) {
        add_number(1, 2, local_depth);
    } else {
        add_number(0, 2, local_depth);
    }
    return rv;
}

std::string& DefaultRndNumGenerator::trace_depth() {
    return trace_string_;
}


unsigned long DefaultRndNumGenerator::genrand(void) {
    return AbsRndNumGenerator::genrand();
}

std::string DefaultRndNumGenerator::RandomHexDigits(int num) {
    // is_random为false情况下，则直接调用AbsRndNumGenreator的生成十六进制数函数获取随机数
    if (!CGOptions::is_random()) {
        return AbsRndNumGenerator::RandomHexDigits(num);
    }
    // is_random为true情况下，需要将每次随机生成的value和bound进行记录，同时对rand_depth记录
    std::string str;
    const char* hex1 = AbsRndNumGenerator::get_hex1();
    while (num--) {
        int x = genrand() % 16;
        str += hex1[x];
        seq_-> add_number(x, 16, rand_depth_);
        rand_depth_++;
    }
    return str;
}

std::string DefaultRndNumGenerator::RandomDigits(int num) {
    // is_random为false的情况下，则直接调用AbsRndNumGenreator的生成十进制数函数获取随机数
    if (!CGOptions::is_random()) {
        return AbsRndNumGenerator::RandomDigits(num);
    }
    std::string str;
    const char* dec1 = AbsRndNumGenerator::get_dec1();
    while (num--) {
        int x = genrand() % 10;
        str += dec1[x];
        seq_->add_number(x, 10, rand_depth_);
        rand_depth_++;
    }
    return str;
}


