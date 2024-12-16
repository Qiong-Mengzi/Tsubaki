/*
 * 一个简易的猜数字工具
 * 我可不想再抽大保底了...
 *
 * 正确的编码组合才能产出指定的物质
 * 每当你设置的编码较上一次更接近正确编码，
 * 机器都会发出比较悦耳的声音
 * 另外有技巧地设置编码将有效地减少生产成本
 * 不过一旦被观测到正确的常数
 * 常数就会被重置
 * 这究竟是神的玩笑还是恶魔的诅咒呢
 *
 * >使用元物质进行制作
 * >调整下方的物品修改你想制作的物品
 *
 * PS:
 * - 常数一共7个位
 * - 初始上一次猜测的数为0 (? <- 待验证)
 */

/*
 * 输入
 * - R: 重置
 * - E: 退出
 * - H: 接近
 * - L: 远离
 */

#include <stdio.h>

inline int abs(int x)
{
    int bits = sizeof(int) - 1;
    return (x ^ (x >> bits)) - (x >> bits);
}

#define MAX_BITS 7
const int _MAX_BITS = 7;

char buf[MAX_BITS + 1] = {};

// 才发现要转成二进制格式
// 感觉笨笨的
 char *get_binary(int dat)
{
    for (int i = 0; i < MAX_BITS; ++i)
        buf[MAX_BITS - i - 1] = '0' + ((dat >> i) & 0x1);
    return buf;
}

typedef struct GuessNum
{
    int last_guess;
    int this_guess;
} GuessNum;

void ResetGuessNum(GuessNum *gn)
{
    gn->last_guess = 0;
    gn->this_guess = (1 << MAX_BITS) - 1;
}

// *核心函数*
// 注释与实际代码有出入，因为实际代码部分有优化过
int test(GuessNum *gn, int result)
{
    int distance = (abs(gn->last_guess - gn->this_guess) + 1) / 2; // 设距离 = 上一次猜的数 与 这一次猜的数 的绝对值  + 1
    int tmp = gn->last_guess;
    gn->last_guess = gn->this_guess;
    if (result > 0) // 如果更近（悦耳/声调高）
    {
        if (tmp > gn->this_guess)
            gn->this_guess = gn->this_guess - distance; // 如果 上一次猜的数 比 这一次 大，就猜 这一次猜的数 减去 距离的一半
        else
            gn->this_guess = tmp + distance; // 否则就猜 上一次猜的数 加上 距离的一半
    }
    else if (result < 0) // 否则（不悦耳/声调低）
    {
        if (tmp < gn->this_guess)
            gn->this_guess = gn->this_guess - distance; // 如果 上一次猜的数 比 这一次 小，就猜 这一次猜的数 减去 距离的一半
        else
            gn->this_guess = tmp + distance; // 否则就猜 上一次猜的数 加上 距离的一半
    }
    else
        ; // 如果出结果了，肯定不需要再猜测了嘛

    return gn->this_guess;
}

int main(int argc, char **argv)
{
    int c;
    GuessNum Gn;
    ResetGuessNum(&Gn);
    fprintf(stdout, "Guess %s: ", get_binary(Gn.last_guess));
    fgetc(stdin);
    fflush(stdin);

    int flag = 1;

    while (flag)
    {
        fprintf(stdout, "Guess %s: ", get_binary(Gn.this_guess));
        c = fgetc(stdin);
        fflush(stdin);
        switch (c)
        {
        case 'H':
            test(&Gn, 1);
            break;
        case 'L':
            test(&Gn, -1);
            break;

        case 'R':
            ResetGuessNum(&Gn);
            fprintf(stdout, "Guess %s: ", get_binary(Gn.last_guess));
            fgetc(stdin);
            fflush(stdin);
            break;
        case 'E':
            flag = 0;
            break;

        default:
            fprintf(stdout, "Unknown Operate \'%c\'\n", c);
            break;
        }
    }
    return 0;
}
