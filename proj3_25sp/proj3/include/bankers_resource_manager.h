#ifndef PROJ3_INCLUDE_BANKERS_RESOURCE_MANAGER_H_
#define PROJ3_INCLUDE_BANKERS_RESOURCE_MANAGER_H_
#include <iostream>
#include <vector>
#include <cstddef>
#include <string>
#include "thread_mutex.h"

class BankersResourceManager{
    public:
    explicit BankersResourceManager(const std::vector<std::size_t>& available);
    void AddMax(const std::vector<std::size_t> &max);
    bool Request(std::size_t id, const std::vector<std::size_t>& request);
    void Release(std::size_t id);

    private:
    std::vector<std::size_t> available_;
    std::vector<std::vector<std::size_t>> maxNeeds_;
    std::vector<std::vector<std::size_t>> allocation_;
    ThreadMutex mutex_;
    bool SafeOrNot( std::vector<std::size_t> NewAvailable,
                 std::vector<std::vector<std::size_t>> NewNeed,
                std::vector<std::size_t>& safeSequence,
                std::vector<std::vector<std::size_t>> newAllocation);
    bool DoesItFit(std::vector<std::size_t> need, std::vector<std::size_t> available);
    void releaseResources(std::vector<std::size_t>& available, std::vector<std::size_t>& alloca);
    void PrintLog(
        std::size_t id,
        const std::vector<std::size_t>& request,
        const std::vector<std::vector<std::size_t>>& needs,
        const std::vector<std::size_t>& safeSequence,
        const std::vector<std::vector<std::size_t>>& NewNeeds,
        const std::vector<std::size_t>& updatedAvailable,bool ans);  

};
#endif // PROJ3_INCLUDE_BANKERS_RESOURCE_MANAGER_H_