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

### Ingress Setup

kubectl get pods,svc -n ingress-space
kubectl get ingressclasses

cat <<EOF > minimal-ingress.yaml
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: minimal-ingress
  namespace: ingress-space
  annotations:
    nginx.ingress.kubernetes.io/rewrite-target: /
spec:
  ingressClassName: nginx
  rules:
  - http:
      paths:
      - path: /blue
        pathType: Prefix
        backend:
          service:
            name: blue-service
            port:
              number: 80
      - path: /green
        pathType: Prefix
        backend:
          service:
            name: green-service
            port:
              number: 80
EOF
kubectl apply -f minimal-ingress.yaml

kubectl get ingress minimal-ingress -n ingress-space
curl -kL http://localhost/blue
curl -kL http://localhost/green

### service nodeport

kubectl get deployments --show-labels
kubectl expose deployment web-imperatif --name=svc-imperatif --type=NodePort --port=80 --target-port=80

apiVersion: v1
kind: Service
metadata:
  name: svc-declaratif
spec:
  type: NodePort
  selector:
    app: web-declaratif <- label du deployment
  ports:
  - port: 80
    targetPort: 80
    nodePort: 30081

kubectl get deployments --show-labels
kubectl expose deployment web-imperatif --name=svc-imperatif --type=NodePort --port=80 --target-port=80
    --name : nom du service à créer
    --type : type du service (NodePort, ClusterIP, LoadBalancer)
    --port : port exposé par le service (côté client interne)
    --target-port : port du conteneur (côté pod)

cat <<EOF > svc-declaratif.yaml
apiVersion: v1
kind: Service
metadata:
  name: svc-declaratif
spec:
  type: NodePort
  selector:
    app: web-declaratif   # Si votre label est app=web-declaratif
  ports:
  - port: 80
    targetPort: 80
    nodePort: 30081
EOF

kubectl apply -f svc-declaratif.yaml

kubectl get svc
NODE_IP=$(kubectl get nodes -o wide | awk 'NR==2{print $6}')
curl http://$NODE_IP:30081

### DNS debug

kubectl get pods -n kube-system -l k8s-app=kube-dns
kubectl exec -it net-debug -- nslookup kubernetes.default
kubectl logs -l k8s-app=kube-dns -n kube-system
kubectl describe configmap coredns -n kube-system
kubectl edit configmap coredns -n kube-system
kubectl rollout restart deployment coredns -n kube-system
kubectl exec -it net-debug -- nslookup kubernetes.default

### network-connectivity

kubectl get networkpolicy backend-lockdown -n connectivity-lab -o yaml
kubectl get pod client -n connectivity-lab --show-labels
kubectl apply -f .
Explications :
podSelector.matchLabels: app: backend — la politique s'applique aux pods backend
ingress[0].from.podSelector.matchLabels: app: client — seuls les pods avec app: client sont autorisés à envoyer du trafic
Tout autre trafic entrant reste bloqué (principe du moindre privilège)

cat <<EOF > policy.yaml
apiVersion: networking.k8s.io/v1
kind: NetworkPolicy
metadata:
  name: backend-lockdown
  namespace: connectivity-lab
spec:
  podSelector:
    matchLabels:
      app: backend
  policyTypes:
  - Ingress
  ingress:
  - from:
    - podSelector:
        matchLabels:
          app: client
EOF
kubectl apply -f policy.yaml

Indice :
Une NetworkPolicy avec policyTypes: [Ingress] et une liste ingress: [] vide signifie rejet total de tout le trafic entrant.
La section ingress doit contenir des règles pour autoriser du trafic.

kubectl get networkpolicy backend-lockdown -n connectivity-lab -o yaml
kubectl get pod client -n connectivity-lab --show-labels

kubectl apply -f.
Explications :
podSelector.matchLabels: app: backend — la politique s'applique aux pods backend
ingress[0].from.podSelector.matchLabels: app: client — seuls les pods avec app: client sont autorisés à envoyer du trafic
Tout autre trafic entrant reste bloqué (principe du moindre privilège)

cat <<EOF > policy.yaml
apiVersion: networking.k8s.io/v1
kind: NetworkPolicy
metadata:
  name: backend-lockdown
  namespace: connectivity-lab
spec:
  podSelector:
    matchLabels:
      app: backend
  policyTypes:
  - Ingress
  ingress:
  - from:
    - podSelector:
        matchLabels:
          app: client
EOF
kubectl apply -f policy.yaml

### service-endpoints

kubectl get svc web-service
kubectl get endpoints web-service
kubectl get pods --show-labels
kubectl describe svc web-service | grep Selector
kubectl edit svc web-service
kubectl get endpoints web-service

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

## Secret

kubectl get secrets

kubectl create secret generic super-secret --from-literal=password=bob  --from-literal=credential=alice

vi secret.yaml

apiVersion: v1
kind: Pod
metadata:
  name: pod-secrets-via-file
spec:
  containers:
  - name: pod-secrets-via-file
    image: redis
    volumeMounts:
    - name: super-secret
      mountPath: /secrets
      readOnly: true
  volumes:
  - name: super-secret
    secret:
      secretName: super-secret

vi pod-secrets-via-env.yaml

apiVersion: v1
kind: Pod
metadata:
  name: pod-secrets-via-env
spec:
  containers:
  - name: redis
    image: redis
    env:
      - name: SECRET_USERNAME
        valueFrom:
          secretKeyRef:
            name: super-secret
            key: username
      - name: SECRET_PASSWORD
        valueFrom:
          secretKeyRef:
            name: super-secret
            key: password
  restartPolicy: Never


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

### kubeadm

# replace x in 1.36.x-* with the latest patch version
sudo apt-mark unhold kubeadm && \
sudo apt-get update && sudo apt-get install -y kubeadm='1.36.x-*' && \
sudo apt-mark hold kubeadm

kubeadm version
sudo kubeadm upgrade plan
sudo kubeadm upgrade apply v1.36.x

sudo kubeadm upgrade node
kubectl drain <node-to-drain> --ignore-daemonsets
sudo apt-mark unhold kubelet kubectl && \
sudo apt-get update && sudo apt-get install -y kubelet='1.36.x-*' kubectl='1.36.x-*' && \
sudo apt-mark hold kubelet kubectl
sudo systemctl daemon-reload
sudo systemctl restart kubelet
kubectl uncordon <node-to-uncordon>

### etcd

ETCDCTL_API=3 etcdctl --endpoints 10.2.0.9:2379 \
  --cert=/etc/kubernetes/pki/etcd/server.crt \
  --key=/etc/kubernetes/pki/etcd/server.key \
  --cacert=/etc/kubernetes/pki/etcd/ca.crt \
  snapshot save <backup-file-location>

export ETCDCTL_API=3
etcdctl --data-dir <data-dir-location> snapshot restore snapshot.db

etcdutl --data-dir <data-dir-location> snapshot restore snapshot.db

### kube config

kubectl config set-cluster kubernetes --server=https://127.0.0.1:6443 --certificate-authority=/etc/kubernetes/pki/ca.crt --embed-certs --kubeconfig=/root/jane.kubeconfig
k config set-credentials jane --client-key /opt/course/15/jane.key --client-certificate /opt/course/15/jane.crt --certificate-authority /etc/kubernetes/pki/ca.crt --embed-certs --kubeconfig /root/jane.kubeconfig 
k config set-context jane --cluster kubernetes --user jane --kubeconfig /root/jane.kubeconfig 
k config use-context jane-context 

### network policies

root@cka-network-policy-cluster1-control-plane:~# cat deny-all-blue.yaml 
apiVersion: networking.k8s.io/v1
kind: NetworkPolicy
metadata:
  name: default-deny-ingress
  namespace: blue
spec:
  podSelector: {}
  policyTypes:
  - Ingress

root@cka-network-policy-cluster1-control-plane:~# k exec hacker -n red -- curl http://backend.svc

### RBAC: test instead of guessing

kubectl auth can-i get pods --as=<user> -n <ns>
kubectl create role pod-reader --verb=get,list,watch --resource=pods -n <ns>
kubectl create rolebinding read-pods --role=pod-reader --user=<user> -n <ns>

### RBAC user
mkdir -p /root/certificates
cd /root/certificates
openssl genrsa -out jane.key 2048
openssl req -new -key jane.key -out jane.csr -subj "/CN=jane"

cat jane.csr | base64 | tr -d "\n"
# Puis copier la valeur affichée
vi csr-jane.yaml 
# Remplacer <insérez-votre-base64-ici> par la valeur copiée
kubectl apply -f csr-jane.yaml

apiVersion: certificates.k8s.io/v1
kind: CertificateSigningRequest
metadata:
  name: jane
spec:
  request: <insérez-votre-base64-ici>
  signerName: kubernetes.io/kube-apiserver-client
  usages:
  - client auth

k get csr
kubectl certificate approve jane
kubectl get csr jane -o jsonpath='{.status.certificate}' | base64 --decode > jane.crt

### serviceaccount-token

kubectl create serviceaccount app-viewer -n sa-lab
kubectl get sa app-viewer -n sa-lab
kubectl run dashboard --image=nginx -n sa-lab --dry-run=client -o yaml > pod.yaml

apiVersion: v1
kind: Pod
metadata:
  creationTimestamp: null
  labels:
    run: dashboard
  name: dashboard
  namespace: sa-lab
spec:
  serviceAccountName: app-viewer   # <--- C'est cette ligne qui fait le lien
  containers:
  - image: nginx
    name: dashboard
    resources: {}
  dnsPolicy: ClusterFirst
  restartPolicy: Never
status: {}

kubectl get pod dashboard -n sa-lab -o yaml | grep serviceAccountName


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


### daemonset-deploy

kubectl get nodes -l disk=ssd
kubectl create deployment ssd-monitor --image=nginx:alpine -n lab-ds --dry-run=client -o yaml > ds.yaml

apiVersion: apps/v1
kind: DaemonSet            # On change le Kind
metadata:
  labels:
    app: ssd-monitor
  name: ssd-monitor
  namespace: lab-ds
spec:                      # Pas de champ "replicas" ou "strategy" ici
  selector:
    matchLabels:
      app: ssd-monitor
  template:
    metadata:
      labels:
        app: ssd-monitor
    spec:
      nodeSelector:        # Ajout du ciblage
        disk: ssd
      containers:
      - image: nginx:alpine
        name: nginx

kubectl get ds -n lab-ds
kubectl get pods -n lab-ds -o wide

### deployment-rollout

kubectl get deployments
kubectl describe deployment web-app
kubectl set image deployment/web-app nginx=nginx:1.16 --record
kubectl rollout status deployment/web-app
kubectl rollout history deployment/web-app
kubectl rollout undo deployment/web-app
kubectl describe deployment web-app | grep Image

### pod-affinity

kubectl get deployment frontend -o yaml | grep podAffinity -A 10
kubectl get pods --show-labels

kubectl edit deployment frontend
      affinity:
        podAffinity:
          requiredDuringSchedulingIgnoredDuringExecution:
          - labelSelector:
              matchExpressions:
              - key: app
                operator: In
                values:
                - redis          # <- Remplacer 'database' par 'redis'
            topologyKey: kubernetes.io/hostname

kubectl get pods -w

### static-pod

grep staticPodPath /var/lib/kubelet/config.yaml
kubectl run static-web --image=nginx --restart=Never --dry-run=client -o yaml > /etc/kubernetes/manifests/static-web.yaml
kubectl get pods -w
kubectl describe pod static-web-<node_name> | head -n 15
\# (Facultatif - tentative de suppression)
kubectl delete pod static-web-<node_name>

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

### PV & PVC Static

kubectl get pods
kubectl describe pod nginx-storage-pod
kubectl get pv,pvc

kubectl delete pvc lab-pvc
cat <<EOF > fix-pvc.yaml
apiVersion: v1
kind: PersistentVolumeClaim
metadata:
  name: lab-pvc
spec:
  storageClassName: manual
  accessModes:
    - ReadWriteOnce
  resources:
    requests:
      storage: 1Gi
EOF
kubectl apply -f fix-pvc.yaml
kubectl get pv,pvc
kubectl get pods


### StorageClass Dynamic

kubectl get pods
kubectl describe pod storage-pod
Explication : "Unbound PVC" est le symptôme classique. Le pod ne trouve pas son stockage ou la requête n'a pas pu être honorée dans ce monde.

kubectl get pvc
kubectl describe pvc web-data-pvc
Explication : La description confirme ce qui manque : un composant nommé fast-storage empêche la fourniture du disque de manière dynamique.

kubectl get sc

cat <<EOF > sc.yaml
apiVersion: storage.k8s.io/v1
kind: StorageClass
metadata:
  name: fast-storage
provisioner: k8s.io/minikube-hostpath   # <-- Ajustez selon ce que 'kubectl get sc' a révélé !
volumeBindingMode: WaitForFirstConsumer
EOF
kubectl apply -f sc.yaml
Explication : Vous agissez ainsi comme un Ops de l'infrastructure. En créant cette passerelle (indiquant quel plugin gérera réellement la demande "fast-storage"), le provisionneur va s'activer.

kubectl get pvc,pv
kubectl get pods

### Volume mount

kubectl get pods
kubectl describe pod volume-pod
Explication : L'historique d'erreur mentionne MountVolume.SetUp failed for volume "secret-volume" : secret "my-app-secret-typo" not found. Le nœud refuse de démarrer le conteneur car il lui manque ce fichier à monter !

kubectl get secrets
Explication : Vous allez constater la présence d'un secret nommé my-app-secret. Le développeur a tapé my-app-secret-typo en rédigeant le YAML, créant ainsi une erreur.

kubectl get pod volume-pod -o yaml > pod.yaml
  volumes:
  - name: secret-volume
    secret:
      defaultMode: 420
      secretName: my-app-secret    # <- Retirer "-typo"

kubectl replace --force -f pod.yaml
kubectl get pods

## PV - PVC - POD

apiVersion: v1
kind: PersistentVolume
metadata:
  name: app-data
spec:
  capacity:
    storage: 2Gi
  volumeMode: Filesystem
  accessModes:
    - ReadOnlyMany
  persistentVolumeReclaimPolicy: Recycle
  storageClassName: slow
  hostPath:
    path: /srv/app-data


https://kubernetes.io/docs/concepts/storage/persistent-volumes/

https://kubernetes.io/docs/tasks/configure-pod-container/configure-persistent-volume-storage/#create-a-persistentvolumeclaim

vi pvc.yaml

apiVersion: v1
kind: PersistentVolumeClaim
metadata:
  name: pv-volume
spec:
  accessModes:
    - ReadWriteOnce
  volumeMode: Filesystem  
  resources:
    requests:
      storage: 10Mi
  storageClassName: csi-hostpath-sc


vi pod-pvc.yaml

apiVersion: v1
kind: Pod
metadata:
  name: web-server
spec:
  containers:
    - name: web-server
      image: nginx
      volumeMounts:
      - mountPath: "/usr/share/nginx/html"
        name: my-volume
  volumes:
    - name: my-volume
      persistentVolumeClaim:
        claimName: pv-volume


kubectl create –f pod-pvc.yaml

kubectl edit pvc pv-volume --record

## Deployment

### Create deployment of nginx-app
kubectl run nginx -app --image=nginx: 1.11 . 0 -alpine - record
### Modify the image, nginx - app is the name of the container
kubectl set image deployment nginx -app nginx-app=nginx: 1.11.3 - alipne
### Rollback
kubectl rollout undo deployment nginx -app
Reference: https://kubernetes.io/docs/reference/kubectl/cheatsheet/

## Pod - NodeSelector

Schedule a pod as follows:
•	name: nginx-kusc00401
•	Image: nginx
•	Node selector: disk-spinning

https://kubernetes.io/docs/concepts/scheduling-eviction/assign-pod-node/

#yaml
apiVersion: v1
kind: Pod
metadata:
  name: nginx-kusc00401
spec:
  containers:
  - name: nginx
    image: nginx
    imagePullPolicy: IfNotPresent
  nodeSelector:
    disk: spinning

kubectl create -f node-select.yaml

## Ingress

https://kubernetes.io/docs/concepts/services-networking/ingress/

vi ingress.yaml

apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: ping
  namespace: ing-internal
spec:
  rules:
  - http:
      paths:
      - path: /hi
        pathType: Prefix
        backend:
          service:
            name: hi
            port:
              number: 5678
kubectl create –f ingress.yaml

## Network Policy

apiVersion: networking.k8s.io/v1
kind: NetworkPolicy
metadata:
  name: allow-port-from-namespace
  namespace: internal
spec:
  podSelector: {}
  ingress:
  - {}
  policyTypes:
  - Ingress
  - from:
    - podSelector: {}
    ports:
    - port: 8080
      protocol: TCP

kubectl create -f network.yaml

## Deployement - Port - Service

kubectl get deployment

kubctl edit deployment front-end

spec:
      containers:
       - image: nginx:1.14.2
        imagePullPolicy: IfNotPresent
        name: nginx
        ports:
        - containerPort: 80
          name: http
          protocol: TCP


kubectl expose deployment front-end –name=front-end-svc –port=80 –target-port=80 –type=NodePort

## Deployement scale

kubectl get deployments 

kubectl scale deployment deployment.apps/presentation –replicas=6

## Sidecar container

aptVerston: v1
kind: Pod
netadata:
name: big-corp-app
spec:
  contatners:
  - nane: count-log
    image: busybox
    args: [/bin/sh, -c, 'tail -n+1 -f /var/log/big-corp-app.log']
    volumeMounts:
      name: varlog
      nountPath: /var/log
   volumes:
   - nane: varlog
     emptyDtr: {}

$ kubectl config use-context k8s
$ kubectl get po big-corp-app -o yaml > big-corp-app.yaml
$ kubectl delete po big-corp-app
$ kubectl apply -f big-corp-app.yaml

## Speed

export do="--dry-run=client -o yaml"
alias k=kubectl
source <(kubectl completion bash)
complete -o default -F __start_kubectl k
