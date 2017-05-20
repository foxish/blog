---
layout: post
title: Kubernetes Cluster Scheduling
---

This post will detail the mechanism and policies which govern scheduling in [Kubernetes](http://kubernetes.io). This post is meant to be technical in nature and assumes familiarity with the Kubernetes system. It reflects the current state of affairs in Kubernetes as of late 2016. In a fast moving project of this nature with over a 1000 unique contributors, new features get added rather quickly. 

Kubernetes performs scheduling via the kube-scheduler binary. It is a core component of Kubernetes and is in charge of scheduling a given pod (smallest unit of scheduling in Kubernetes) and assigning them to specific nodes.

The sources for the scheduler can be found at [plugin/pkg/scheduler](https://github.com/kubernetes/kubernetes/tree/7b134995e5b10d4d442430838a9ebb8305d9e660/plugin/pkg/scheduler). First, we shall describe the policies and then the mechanism by which these policies are implemented.

### Policies

The Kubernetes scheduler operates on a simple reconciliation loop like other components of Kubernetes. It is a separate process run on the master node, alongside the API Server and other components. It "watches" Pods with no Node name assigned to them, and pods a binding that indicates the node on which that Pod should be scheduled [1]. 

At a high level, the scheduler operates as follows:

1. First it applies a set of "predicates" to filter out inappropriate nodes. For example, if the PodSpec specifies resource requests, then the scheduler will filter out nodes that don't have at least that much resources available (computed as the capacity of the node minus the sum of the resource requests of the containers that are already running on the node).

2. Second, it applies a set of "priority functions" that rank the nodes that weren't filtered out by the predicate check. For example, it tries to spread Pods across nodes and zones while at the same time favoring the least (theoretically) loaded nodes (where "load" - in theory - is measured as the sum of the resource requests of the containers running on the node, divided by the node's capacity).

3. Finally, the node with the highest priority is chosen (or, if there are multiple such nodes, then one of them is chosen at random).


### Mechanism

The scheduler is organized as a set of algorithm plugins which provide an extensible way to configure it. Each algorithm provider comes with a way to configure predicates and priority functions, as well as a mechanism to assign relative weights to different priority functions.

The algorithm provider to use can be chosen at runtime using a [command-line argument](http://kubernetes.io/docs/admin/kube-scheduler/) that can be passed to the scheduler binary. The default algorithm provider is called the DefaultProvider. 


#### Predicates

##### Basic Predicates

PodFitsResources: This checks the resource requirement of an unscheduled pod against the [*allocatable*](https://github.com/kubernetes/community/blob/f4379b3c775e31b38df46fb174cdafceceaaca33/contributors/design-proposals/node-allocatable.md) resources on that node. Here, resources could mean the following:
   * [MilliCPU](http://kubernetes.io/docs/user-guide/compute-resources/#meaning-of-cpu)          
   * [Memory](http://kubernetes.io/docs/user-guide/compute-resources/#meaning-of-memory)
   * NvidiaGPU
   * [OpaqueIntResources](http://kubernetes.io/docs/user-guide/compute-resources/#opaque-integer-resources-alpha-feature)

PodFitsHost: This predicate checks if the node being considered for scheduling is a valid one.

PodFitsHostPorts: This predicate checks if the [HostPort](https://github.com/kubernetes/kubernetes/blob/91931c138edcbd13613b2f63ebe191be6ae69814/pkg/api/v1/types.go#L1124-L1129) type of ports requested by a particular pod are in conflict with ports already open on that host. Note that the use of HostPort is discouraged.

PodSelectorMatches: This predicate checks if there is an explicit `NodeSelector` configured with the pod, which precludes the scheduler from scheduling that pod on nodes which do not match that selector.

##### Other Predicates

Some other predicates that prove useful with different providers are:













References:

[1]: https://github.com/kubernetes/community/blob/2bb1e565da14089f574657f7cd83b4c20d244b59/contributors/devel/scheduler.md

[2]: 
[3]: 




