# Debugging Worksheet: Broken Code Challenge

Name: ___________________________

## Part 1: Before Debugging

Read the program before running it.

### 1. What is the program supposed to do?



### 2. What output do you expect if the program works correctly?



---

## Part 2: Using Breakpoints

Set a breakpoint inside this function:

```cpp
double computeAverage(std::vector<int>& nums)
```

### 3. What line did you choose for your breakpoint?



### 4. When the debugger pauses, what are the values of these variables?

| Variable | Value |
|---|---|
| `nums.size()` | |
| `sum` | |
| `i` | |

### 5. Does `sum` start with the value you expected? Explain.



---

## Part 3: Debugging the Loop

Step through the loop one line at a time.

### 6. What values does `i` take as the loop runs?



### 7. What happens when `i == nums.size()`?



### 8. What should the loop condition be instead?



---

## Part 4: Debugging the Average

After fixing the loop and `sum`, inspect the return value from `computeAverage`.

### 9. What average does the function return?



### 10. Is the division using integer division or floating-point division?



### 11. How can you fix the division?



---

## Part 5: Debugging the Filter Function

Set a breakpoint inside:

```cpp
std::vector<int>& filterAboveAverage(std::vector<int> nums, double avg)
```

### 12. What values are added to `result`?



### 13. What is wrong with returning `result` by reference?



### 14. What should the function return instead?



---

## Part 6: Final Fixes

### 15. List each bug you found and how you fixed it.

| Bug | Fix |
|---|---|
| 1 | |
| 2 | |
| 3 | |
| 4 | |
| 5 | |
| 6 | |

---

## Part 7: Reflection

### 16. Which bug was easiest to find with the debugger?



### 17. Which bug was hardest to understand?



### 18. What debugger feature helped you the most?

- Breakpoints
- Step Over
- Step Into
- Step Out
- Variables panel
- Watch expressions
- Hovering over variables

Explain briefly:


