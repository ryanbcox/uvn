This repository contians scripts and data used for a paper I wrote. See:
https://tech.ryancox.net/2022/01/network-segmentation-of-users-on-multi.html
https://www.sans.org/white-papers/40090/
https://byu-my.sharepoint.com/:p:/g/personal/ryancox_byu_edu/Ee2uW-NXlylMndFcB9StvF4BElZ2agFWI3-PYZ6C7GJ03A?e=5bEyqS (Select the "View" menu within the browser-based PowerPoint's menu then click "Notes" to see the copious speaker notes I was asked to include.)

Configuration can be as simple as adding these lines:
```
/etc/authselect/postlogin: 
session  optional  pam_exec.so /usr/local/sbin/create_uvn 

/etc/slurm/plugstack.conf.d/create_uvn.conf: 
optional /usr/local/spank/create_uvn.so 
```
