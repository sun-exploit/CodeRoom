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

## kind: Service

spec:,  type: ClusterIP = interne au cluster

spec:,  type: NodePort = port ouvert sur les Nodes

spec:,  type: LoadBalancer = exposition externe via un Load Balancer

| Élément         | Question à laquelle il répond                                     |
| --------------- | ----------------------------------------------------------------- |
| `kind: Service, spec:  selector: app: web`    | **Quels Pods possèdent ce metadata:, labels: ?** |
| `spec ports: - port: XX  targetPort: XX`      | **Sur quel port le Service écoute ?**    |
| `spec ports: - port: 80 targetPort: 8080`     | **Sur quel port du Pod envoyer ?**       |
| `EndpointSlice` | **Quelles sont les adresses réelles des endpoints disponibles ?** |

DNS : service.namespace.svc.cluster.local

kubectl run dns-test --image=busybox:1.36 --restart=Never --rm -it -- sh
> nslookup my-service # nslookup my-service.my-namespace # nslookup my-service.my-namespace.svc.cluster.local

> kubectl get pods -n kube-system -l k8s-app=kube-dns
> kubectl get svc -n kube-system kube-dns
> kubectl exec <pod> -- cat /etc/resolv.conf
> kubectl get svc
> kubectl get endpointslice
> kubectl exec -it <pod> -- nslookup <service>

## kind : Ingress

Un Ingress définit les règles de routage HTTP/HTTPS. 
L'Ingress Controller est le composant qui applique réellement ces règles.

L'Ingress peut être parfaitement configuré mais le Service ne possède aucun Pod correspondant au selector

spec:,                          rules:, - host , http:, paths:, - path:
spec:, ingressClassName: nginx, rules:, - host:

kubectl get ingress
kubectl describe ingress <name>
kubectl get ingressclass
kubectl get svc

| Élément                | Rôle                                         |
| ---------------------- | -------------------------------------------- |
| **Ingress**            | Décrit les règles HTTP/HTTPS                 |
| **Ingress Controller** | Applique réellement ces règles               |
| **IngressClass**       | Indique quel Controller doit gérer l'Ingress |
| **Service**            | Fournit le backend vers lequel router        |
| **EndpointSlice**      | Liste les endpoints disponibles              |
| **Pod**                | Exécute réellement l'application             |


                    Client
                      │
                 HTTP / HTTPS
                      │
                      ▼
              Ingress Controller
                      │
              lit les règles
                      │
                      ▼
                  Ingress
                /          \
               /            \
              ▼              ▼
         Service A       Service B
             │                │
             ▼                ▼
        EndpointSlice    EndpointSlice
             │                │
             ▼                ▼
           Pods             Pods
           
## Gateway API : Resources et Routing

| Resource        | Question                                |
| --------------- | --------------------------------------- |
| `GatewayClass`  | **Quel Controller ?**                   |
| `Gateway`       | **Quel point d'entrée / quels ports ?** |
| `listener`      | **Quel protocole/port ?**               |
| `HTTPRoute`     | **Comment router ?**                    |
| `parentRefs`    | **À quelle Gateway ?**                  |
| `backendRefs`   | **Vers quel Service ?**                 |
| `EndpointSlice` | **Vers quels Pods ?**                   |

GatewayClass choisit l'implémentation, Gateway expose le point d'entrée, HTTPRoute définit le routage, et backendRefs envoie le trafic vers les Services.

kubectl get gatewayclass
kubectl get gateway
kubectl get httproute
kubectl describe gateway <name>
kubectl describe httproute <name>

## RBAC

Role - quoi ?
  → permissions dans un namespace

ClusterRole - dans le cluster - kind: ClusterRole, metadata:, name: pod-reader, rules:
  → permissions cluster-wide
    (mais peut aussi être utilisé via RoleBinding)

RoleBinding - dans quel namespace ?
  → attribue un Role/ClusterRole dans un namespace

ClusterRoleBinding - partout ?
  → attribue un ClusterRole dans tout le cluster

kubectl auth can-i <verb> <resource> --as=<user> -n <namespace>

## Helm and Kustomize for installing or updating components

                  Kubernetes configuration
                          │
              ┌───────────┴───────────┐
              ▼                       ▼
            Helm                  Kustomize
              │                       │
         Chart + values          YAML + overlays
              │                       │
              ▼                       ▼
       helm install/upgrade     kubectl apply -k
              │                       │
              └───────────┬───────────┘
                          ▼
                     Kubernetes

helm repo add <name> <url>
helm repo update
helm search repo <keyword>

helm install <release> <chart>
helm upgrade <release> <chart>

helm list
helm status <release>

helm history <release>
helm rollback <release> <revision>

helm uninstall <release>

helm template <release> <chart>
**helm template <release> <chart>**

kubectl kustomize <directory>
kubectl apply -k <directory>
kubectl delete -k <directory>
**kubectl kustomize <directory>**

## CNI, CSI, and CRI responsibilities

                       kubelet
                          │
             ┌────────────┼────────────┐
             │            │            │
             ▼            ▼            ▼
            CRI          CNI          CSI
             │            │            │
             ▼            ▼            ▼
        containerd     Pod Network   Storage
        / CRI-O            │            │
             │             ▼            ▼
             ▼            IP          Volume
        Containers

kubectl get pods -o wide   → réseau / CNI
kubectl get csidrivers     → stockage / CSI
crictl ps                  → runtime / CRI

## Cluster administration: control the control plane

### RBAC: test instead of guessing

kubectl auth can-i get pods --as=<user> -n <ns>
kubectl create role pod-reader --verb=get,list,watch --resource=pods -n <ns>
kubectl create rolebinding read-pods --role=pod-reader --user=<user> -n <ns>

### Node maintenance

kubectl cordon <node>
kubectl drain <node> --ignore-daemonsets --delete-emptydir-data
# perform maintenance
kubectl uncordon <node>

### Backups and upgrades

## Workloads and scheduling: generate, edit, verify

kubectl create deployment web --image=nginx --dry-run=client -o yaml > web.yaml
kubectl run test --image=busybox:1.36 --restart=Never --dry-run=client -o yaml > pod.yaml
kubectl expose deployment web --port=80 --target-port=8080 --dry-run=client -o yaml > svc.yaml

kubectl rollout status deployment/<name> -n <ns>
kubectl rollout history deployment/<name> -n <ns>
kubectl rollout undo deployment/<name> -n <ns>
kubectl get pod -n <ns> -o wide

## Storage: understand the binding story

kubectl get sc,pv,pvc -A
kubectl describe pvc <pvc> -n <ns>
kubectl describe pv <pv>
kubectl describe pod <pod> -n <ns>

apiVersion: v1
kind: PersistentVolumeClaim
metadata:
  name: data
spec:
  accessModes:
    - ReadWriteOnce
  resources:
    requests:
      storage: 10Gi

kubectl get pvc
kubectl describe pvc <name>
kubectl get pv
kubectl describe pv <name>
kubectl get storageclass
kubectl describe storageclass <name>
kubectl get csidrivers

## Speed

export do="--dry-run=client -o yaml"
alias k=kubectl
source <(kubectl completion bash)
complete -o default -F __start_kubectl k
