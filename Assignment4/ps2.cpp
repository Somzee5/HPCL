#include <iostream>
#include <queue>
#include <omp.h>
using namespace std;

const int buffer = 5;  
const int cnt = 10; 

queue<int> buff;  
int producer_item = 0;
int consumer_item = 0;

int main() {
    omp_set_num_threads(2); 
    double start = omp_get_wtime();

    #pragma omp parallel sections shared(buff, producer_item, consumer_item)
    {
        #pragma omp section
        {
            while (producer_item < cnt) {
                #pragma omp critical
                {
                    if (buff.size() < buffer) {
                        int item = producer_item + 1;
                        buff.push(item);
                        producer_item++;
                        cout << "Producer produced: " << item << "\n";
                    }
                }
            }
        }

        
        #pragma omp section
        {
            while (consumer_item < cnt) {
                #pragma omp critical
                {
                    if (!buff.empty()) {
                        int item = buff.front();
                        buff.pop();
                        consumer_item++;
                        cout << "Consumer consumed: " << item << endl;
                    }
                }
            }
        }
    }

    double end = omp_get_wtime();
    cout << "Total time: " << (end - start) << " sec\n";

    return 0;
}