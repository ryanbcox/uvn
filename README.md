This repository contians scripts and data used for a paper I wrote. I will link it here later and add a summary.

Configuration can be as simple as adding these lines:
```
/etc/authselect/postlogin: 
session  optional  pam_exec.so /usr/local/sbin/create_uvn 

/etc/slurm/plugstack.conf.d/create_uvn.conf: 
optional /usr/local/spank/create_uvn.so 
```
