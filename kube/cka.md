# Sources
https://medium.com/stackademic/the-last-minute-cka-crash-course-what-to-know-before-you-start-the-exam-1cd488630316


# Intro

Unfortunately, there is no cka-dojo for CKA. It was a more difficult to find practice questions for CKA. These are 2 practices that I found that had many similar questions appear in the exam. CKA-PREP-2025-v2 comes with Youtube videos as well to explain the questions and solutions.

Similar to CKAD, CKA has killer.sh simulation exams too.

The CKAD killer.sh simulation exams pack many tasks in each question. You may know how to do all questions, but the challenge is completing all questions within the time limit.

As for CKA killer.sh simulation exams, they are much more technically challenging. It is not about the time, but whether you even know how to do the question. I barely passed both CKA killer.sh simulation exams and there were a ton of questions that I did not know how to solve.

I took about 3 weeks to prepare for CKA as I gained quite a bit of Kubernetes proficiency by studying for CKAD.

# First, know the weight of the exam

The current curriculum is divided into five domains:

    Troubleshooting — 30%
    Cluster Architecture, Installation & Configuration — 25%
    Services & Networking — 20%
    Workloads & Scheduling — 15%
    Storage — 10%

The obvious lesson: troubleshooting, cluster administration, and networking deserve most of your final revision time. Do not spend your last evening memorizing obscure YAML fields while your service-debugging workflow is still slow.

https://medium.com/stackademic/the-last-minute-cka-crash-course-what-to-know-before-you-start-the-exam-1cd488630316

# CKA

## 001 - Pod Pending

### Symptôme
Pod bloqué en Pending.

### Diagnostic
kubectl describe pod ...

### Cause
Taint sans toleration.

### Solution
Ajouter une toleration.

### À retenir
Toujours regarder Events dans describe.

## Practices: 
https://github.com/vj2201/CKA-PREP-2025-v2 and https://github.com/markdjones82/CKA-PREP-2025-v2/tree/custom-main
https://github.com/TiPunchLabs/ckad-dojo/blob/main/QUICKSTART.md
https://github.com/vj2201/killercoda-scenarios

## Killer.sh 
simulation exams: https://killer.sh/cka

## Sources
https://medium.com/@clovisc/acing-your-cka-and-ckad-exams-8b3ef0441828

## Miscellaneous things

 For every mistake and question that you do not know how to do, make sure you note them down and revise them periodically. Do not just “fire and forget”, that is, read the solutions and forget what you learnt
 Many people have mentioned that you need to use the imperative way of deploying Kubernetes resources (using kubectl to create pods, deployments, services etc). Otherwise, you would not have sufficient time. I disagree. I love using the declarative way of deploying Kubernetes resources and I had sufficient time for both exams. Although certain imperative ways of deploying resources, such as `kubectl expose` saves a lot of time, you do not need to strictly use the imperative way all the time. Analyse the task and see which method is more efficient, rather than stick to one method
