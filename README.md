# From graph based to stream based algorithms

Recently, I have heard about programing task that a company asked to accomplish as a test for developers.

I found the task interesting and started to think on how I could solve it.
The first approach that came in my mind was – utilization of topological sort for formula ordering and calculation. 

Later, during implementation of first approach a question raised in my head – would it be possible to solve this task in parallel fashion. 
First ideas were around decomposition of graph at trees and branches, but that approach seemed to be not very elegant because of complex synchronization between threads. 
So after some thinking my mind produced streaming approach as a solution for the task.

Here is implementation of parpallel and non-parallel solution of spreadsheet formulas calculation task.

Ref: https://whataboutalgo.wordpress.com/2017/04/10/from-graph-based-to-stream-based-algorithms/
