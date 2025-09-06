```
g++ -std=c++17 main.cpp -o minigrep
```

```
cat testfiles/test2.txt | ./minigrep "mary"
```

```
./minigrep -i "stuff" testfiles/test1.txt
```

```
./minigrep "stuff" testfiles/test1.txt

```

```
./minigrep "stuff" testfiles/test2.txt
```

```
./minigrep -c -i "stuff" testfiles/test2.txt
```

```
./minigrep -i -c "stuff" testfiles/test2.txt
```
