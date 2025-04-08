#include "bankers_resource_manager.h"

BankersResourceManager::BankersResourceManager(const std::vector<std::size_t>& available){
    available_ = available;
    maxNeeds_ = std::vector<std::vector<std::size_t>>();
    allocation_ = std::vector<std::vector<std::size_t>>();
}

void BankersResourceManager::AddMax(const std::vector<std::size_t>& max){
    maxNeeds_.push_back(max);
    allocation_.push_back(std::vector<std::size_t>(max.size(),0));
}

bool BankersResourceManager::Request(std::size_t id, const std::vector<std::size_t>& request){
    ThreadMutexGuard guard(mutex_);
    std::vector<std::size_t> remaining(maxNeeds_.at(id).size(),0);
    for(std::size_t i=0;i<allocation_.at(id).size();i++){
        remaining[i] = maxNeeds_.at(id)[i] - allocation_.at(id)[i];
    }
    bool check = true;
    for(std::size_t i=0;i<remaining.size();i++){
        if(request[i]>remaining[i]){
            check=false;
            break;
        }
    }
    std::vector<std::size_t> TempIDAlloc = allocation_.at(id);
    std::vector<std::size_t> updatedAvailable = available_;
    for(std::size_t i=0;i<request.size();i++){
        if(request[i]>available_[i]){
            check=false;
            break;
        }
        else{
            updatedAvailable[i] = available_[i]-request[i];
        }
    }
    std::vector<std::vector<std::size_t>> needs(maxNeeds_.size(),std::vector<std::size_t>(maxNeeds_.at(id).size()));
        for(std::size_t i=0; i<maxNeeds_.size();i++){
            for(std::size_t j=0; j<maxNeeds_.at(i).size();j++){
                needs[i][j]= maxNeeds_[i][j]-allocation_[i][j];
            }
        }
        std::vector<std::size_t> safeSequence;

        for(std::size_t i=0;i<request.size();i++){
           TempIDAlloc[i] += request[i];
        }
        std::vector<std::vector<std::size_t>> updatedAllocation = allocation_;
        updatedAllocation[id] = TempIDAlloc;
        std::vector<std::vector<std::size_t>> NewNeeds(maxNeeds_.size(),std::vector<std::size_t>(maxNeeds_.at(id).size()));
        for(std::size_t i=0; i<maxNeeds_.size();i++){
            for(std::size_t j=0; j<maxNeeds_.at(i).size();j++){
                NewNeeds[i][j]= maxNeeds_[i][j]-updatedAllocation[i][j];
            }
        }
    if(check){
        bool ans = SafeOrNot(updatedAvailable,NewNeeds,safeSequence,updatedAllocation);
        if(ans){
            PrintLog(id,request,needs,safeSequence,NewNeeds,updatedAvailable,ans);
            allocation_ = updatedAllocation;  
            available_ = updatedAvailable;
        }
        else{
            PrintLog(id,request,needs,safeSequence,NewNeeds,updatedAvailable,ans);
        }
        return ans;
    }
    PrintLog(id,request,needs,safeSequence,NewNeeds,updatedAvailable,check);
    return false;
    
}

void BankersResourceManager::Release(std::size_t id){
    ThreadMutexGuard guard(mutex_);
    for(std::size_t i=0; i<available_.size();i++){
        available_[i]+=allocation_.at(id)[i];
        allocation_.at(id)[i] = 0;
    }
}
bool BankersResourceManager::SafeOrNot(std::vector<std::size_t> updatedAvailable, 
                                       std::vector<std::vector<std::size_t>> need,
                                       std::vector<std::size_t>& safeSequence,
                                       std::vector<std::vector<std::size_t>> updatedAllocation){
    std::vector<std::size_t> pendingProcesses;
    for (std::size_t i = 0; i < need.size(); i++) {
        pendingProcesses.push_back(i);
    }
    bool madeProgress = true;
    while (!pendingProcesses.empty() && madeProgress) {
    madeProgress = false;
    for (auto it = pendingProcesses.begin(); it != pendingProcesses.end(); ) {
        std::size_t id = *it;
        if (DoesItFit(need[id], updatedAvailable)) {
            releaseResources(updatedAvailable, updatedAllocation[id]);
            safeSequence.push_back(id);
            it = pendingProcesses.erase(it);
            madeProgress = true;
        } 
        else {
            ++it;
        }
    }
 }
    
    return pendingProcesses.empty();
           
}


void BankersResourceManager::PrintLog(
        std::size_t id,
        const std::vector<std::size_t>& request,
        const std::vector<std::vector<std::size_t>>& needs,
        const std::vector<std::size_t>& safeSequence,
        const std::vector<std::vector<std::size_t>>& NewNeeds,
        const std::vector<std::size_t>& updatedAvailable,
        bool ans){
            if(ans){
                std::cout << "Thread "<< id<<" requested: { ";
                for(std::size_t i=0;i<request.size();i++){
                    std::cout << request[i]<<" ";
                }
                std::cout << "}"<<std::endl;
                std::cout << "Need: {";
                for(std::size_t i=0;i<needs.at(id).size();i++){
                    std::cout<<needs.at(id)[i]<<" ";
                }
                std::cout<<"}"<<std::endl;
                std::cout << "Available: {";
                for(std::size_t i=0;i<available_.size();i++){
                    std::cout<<available_[i]<<" ";
                }
                std::cout<<"}"<<std::endl;
                std::cout<<" Safe. Request allocated. Order: { ";
                for(std::size_t i=0;i<safeSequence.size();i++){
                    std::cout<<"P"<<safeSequence[i]<<" ";
                }
                std::cout<<"}"<<std::endl; 
                std::cout<<"Available: {";
                for(std::size_t i=0;i<updatedAvailable.size();i++){
                    std::cout<<updatedAvailable[i]<<" ";
                } 
                std::cout<<"}"<<std::endl;
                std::cout<<"Need: {";
                for(std::size_t i=0;i<NewNeeds.at(id).size();i++){
                    std::cout<<NewNeeds.at(id)[i]<<" ";
                }
                std::cout<<"}\n"<<std::endl;
            }
            else{
                std::cout << "Thread "<< id<<" requested: { ";
                for(std::size_t i=0;i<request.size();i++){
                    std::cout << request[i]<<" ";
                }
                std::cout << "}"<<std::endl;
                std::cout << "Need: {";
                for(std::size_t i=0;i<needs.at(id).size();i++){
                    std::cout<<needs.at(id)[i]<<" ";
                }
                std::cout<<"}"<<std::endl;
                std::cout << "Available: {";
                for(std::size_t i=0;i<available_.size();i++){
                    std::cout<<available_[i]<<" ";
                }
                std::cout<<"}"<<std::endl;
                std::cout<<"Not Available, request denied.\n"<<std::endl;
                }
        }

bool BankersResourceManager::DoesItFit(std::vector<std::size_t> need, std::vector<std::size_t> available){
    bool ans = true;
    for(std::size_t i= 0;i<available.size();i++){
        if(need[i]>available[i]){
            ans=false;
        }
    }
    return ans;
}

void BankersResourceManager::releaseResources(std::vector<std::size_t>& available, std::vector<std::size_t>& alloca){
    for(std::size_t i=0;i<available.size();i++){
        available[i]+=alloca[i];
    }
}