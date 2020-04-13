#ifndef FILTER_H
#define FILTER_H

#include <bitset>

// 过滤器种类
enum FilterKind {
    fDefault,
    fDFS,
};

// 过滤器种类的最大值
#define MAX_FILTER_KIND_SIZE ((FilterKind) (fDFS + 1))

// Filter 基类
class Filter {
public:
    Filter();
    virtual ~Filter(void);
    virtual bool filter(int v) const = 0;
    
    // 设置第kind模式为可用
    void enable(FilterKind kind);
    // 设置第kind模式不可用
    void disable(FilterKind kind);

protected:
    // 验证当前过滤器模式是否有效
    bool valid_filter() const;
    
    //此过滤器可以应用于哪种模式
    //默认情况下，它可以在所有模式下工作。
    std::bitset<MAX_FILTER_KIND_SIZE> kinds_;

private:
    // 获取当前的过滤器种类
    FilterKind current_kind() const;
};

#endif // FILTER_H