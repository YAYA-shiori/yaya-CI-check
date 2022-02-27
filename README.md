# yaya-CI-check  
 > Promise me not to build the nar when the dic file is wrong, okay?  

exe's source code in [exe branch](https://github.com/Taromati2/yaya-CI-check/tree/exe)  
about `github action`: https://docs.github.com/actions  

useage if check only: https://github.com/Taromati2/ghost/blob/master/.github/workflows/auto_check.yml  
```yml
#//...
runs-on: windows-latest
steps:
  #//...
  - name: shiori-check
    uses: Taromati2/yaya-CI-check@v1
    with:
      shiori-path: .\master\shiori\aya.dll
```
useage if check defore release: https://github.com/Taromati2/package-factory/blob/master/.github/workflows/auto_release.yml  
```yml
jobs:
  check:
    #//...
    runs-on: windows-latest
    steps:
      #//...
      - name: shiori-check
        uses: Taromati2/yaya-CI-check@v1
        with:
          shiori-path: .\Taromati2\ghost\master\shiori\aya.dll
      #//...
  build:
    #//...
    needs: check
    #//...
```

# Attention!  
This check will perform a load of yaya.dll, so the variable file will be created  
We recommend that the check for ghost and the creation of the nar are done separately in different github actions jobs  
or check for ghost after the creation of the nar  
