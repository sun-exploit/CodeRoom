Disk read_ahead: 

The read_ahead parameter speeds up the disk read operation by prefetching data and loading it to random access memory. 

Setting up a relatively higher value for read_ahead will beneit clients performing sequential read operations. 

You can check the current  read_ahead value using this command:
```
# cat /sys/block/vda/queue/read_ahead_kb
```
To set read_ahead to a higher value, execute the following command:

```
# echo "8192">/sys/block/vda/queue/read_ahead_kb
```

Typically, customized read_ahead settings are used on Ceph clients that use RBD. You should change read_ahead for all the RBDs mapped to this host; also make sure to use the correct device path name.
