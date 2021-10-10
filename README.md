# yaya-CI-check  
 > Promise me not to build the nar when the dic file is wrong, okay?  

useage if check only: https://github.com/Taromati2/ghost/blob/master/.github/workflows/auto_check.yml  
useage if check defore release: https://github.com/Taromati2/package-factory/blob/master/.github/workflows/auto_release.yml  

# Attention!  
This check will perform a load of yaya.dll, so the variable file will be created  
We recommend that the check for ghost and the creation of the nar are done separately in different github actions jobs  
or check for ghost after the creation of the nar  
