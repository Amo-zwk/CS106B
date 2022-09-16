/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <iostream>
#include "testing/SimpleTest.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/*
 * 进行优化,利用约数的分布规律，X的开平方前后，约数成对出现，所以只需要计算开平方之前的数即可,可以把范围优化到根号n,所以我们可以通过找根号n前面的因数找出根号n后面所有的因数
 */
long smarterSum(long n) {
    long sum = 0;
    for (long i = 1; i <= n / i; i++) { //因数的范围是1到根号n,进行优化,因数是成对出现的,i <= sqrt(n)由于每次都要计算sqrt,比较慢. 而 i * i <= n容易溢出,这样可以避免TLE
        if (n % i == 0) { //如果是因素,其中一个因数是i(在根号n的左边),另外一个因数是n/i(在根号n的右边)
            if (i < n) { //排除1是完全数
                 sum += i;
            }
            if ( n / i != n && n / i < n) { //排除本身和重复的因数
                sum += n / i;
            }
        }
    }
    return sum;
}

/*
 * 判断是否等于本身
 */
bool isPerfectSmarter(long n) {
    return (n != 0 && n == smarterSum(n));
}

/*
 * 找出某个1-stop内的完全数
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/*
 * 返回第n个完全数(欧几里得优化版本)
 * 梅森数满足: 2^p - 1 (其中p是素数),如果得到素数,那么(2^p) * (2^p-1)就是完全数
 */
long findNthPerfectEuclid(long n) {
    long res = 0, cnt = 0;
    for (long i = 1; i <= 63; i ++) { //long的范围是最大为2^63-1
        if (smarterSum(i) == 1) { //如果i是素数,因为梅森数要满足2^p-1,其中p是素数
            if (smarterSum(pow(2,i)-1) == 1) { //如果这个梅森数是素数:即2^p-1是素数
                res = pow(2,i-1) * (pow(2,i) - 1); //那么(2^p) * (2^p-1)就是完全数
                cnt += 1; //记录完全数的个数
            }
            if (cnt == n) { //如果完全数的个数等于n,跳出循环
                break;
            }
        }
    }
    return res;
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}

// TODO: add your student test cases here

PROVIDED_TEST("Confirm the first perfect number is 6") {
    EXPECT(findNthPerfectEuclid(1));
}
