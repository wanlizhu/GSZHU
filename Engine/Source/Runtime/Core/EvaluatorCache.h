#pragma once

#include <memory>
#include <unordered_map>

namespace NTGS {
    template<typename ReturnType, typename... ArgsType>
    class EvaluatorCache {
    public:
        typedef std::function<Return(ArgsType...)> Evaluator;
        typedef std::unordered_map<std::tuple<ArgsType...>, ReturnType> CachedResults;
    
        EvaluatorCache(const Evaluator& eval) 
            : m_eval(eval), m_cache(new CachedResults()) {}
        
        inline ReturnType operator()(ArgsType... args) {
            std::tuple<ArgsType...> key(args...);
            auto it = m_cache->find(key);

            if (it != m_cache->end()) 
                return it->second;
            
            auto result = eval(ArgsType...);
            m_cache[key] = result;

            return result;
        }

        inline void clear() { m_cache.clear(); }
        inline size_t size() const { return m_cache->size(); }
        inline operator bool() const { return size() > 0; }

    private:
        thread_local static Evaluator m_eval;
        thread_local std::unique_ptr<CachedResults> m_cache;
    };
}