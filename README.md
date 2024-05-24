# Matrix Transposition Algorithms

This repository contains implementations of two matrix transposition algorithms: a cache-oblivious algorithm and a cache-aware algorithm. Both algorithms are designed to transpose a matrix `B` from an input matrix `A`, but they differ in their approach to optimizing cache usage.

## Cache-Oblivious Algorithm

The cache-oblivious algorithm is designed to perform well across various cache configurations without any specific knowledge of the cache architecture. It recursively divides the matrix into smaller submatrices until a base case is reached, performing a simple transpose on these small submatrices. This divide-and-conquer approach helps to optimize cache usage by improving spatial locality and reducing cache misses in a general manner.

### Key Features
- **Universality**: The algorithm does not rely on specific cache parameters, making it adaptable to different cache configurations.
- **Recursive Structure**: It divides the problem into smaller subproblems, which can fit better in the cache, thereby optimizing cache performance indirectly.

## Cache-Aware Algorithm

The cache-aware algorithm, in contrast, is optimized with specific knowledge of the cache architecture, such as the cache size and block size. By using blocking techniques, the algorithm processes the matrix in smaller blocks that fit well within the cache, minimizing cache misses more effectively than the cache-oblivious approach.

### Key Features
- **Cache Optimization**: By knowing the cache parameters, the algorithm can reduce cache misses significantly.
- **Blocking Technique**: The matrix is processed in smaller blocks, which helps to keep the cache lines hot and improve spatial and temporal locality.

## Comparison
### Performance
- **Cache-Oblivious Algorithm**: Generally performs well across different cache configurations but may not achieve the same level of performance as cache-aware algorithms in specific scenarios due to the lack of detailed cache information.
- **Cache-Aware Algorithm**: Typically outperforms cache-oblivious algorithms in scenarios where cache parameters are known, as it can leverage specific optimizations to minimize cache misses.
### Adaptability
- **Cache-Oblivious Algorithm**: Highly adaptable and can be used in various environments without modification.
- **Cache-Aware Algorithm**: Requires knowledge of the cache architecture to achieve optimal performance, which can limit its adaptability across different systems.
#### Performance Metric:
<img src="https://github.com/vinzmoke-zoro/Cache-oblivious-vs-Cache-aware/assets/63388102/d86b2161-8cb4-4655-b8ff-d94b92b55e94" alt="Top Image" style="width: 49%;">
<img src="https://github.com/vinzmoke-zoro/Cache-oblivious-vs-Cache-aware/assets/63388102/119c07af-399d-49e4-bbce-8d08d48227f2" alt="Bottom Image" style="width: 49%;">
<br>
As seen from the performance metric, Cache-oblivious algorithms are versatile and do not require detailed cache information, making them suitable for a wide range of applications. However, they may not achieve the highest possible performance in specific cache configurations. Cache-aware algorithms, on the other hand, can be highly optimized for specific cache architectures, leading to better performance in those scenarios. The choice between the two approaches depends on the specific requirements and constraints of the application at hand.
