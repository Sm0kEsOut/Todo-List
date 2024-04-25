#include <stdio.h>
#include <math.h>

int main()
{
    char operator;
    double firstNumber, secondNumber;

    printf("Enter an operator (+, -, *, /, ^, r): ");
    scanf(" %c", &operator);

    if (operator == 'r')
    {
        printf("Enter the number: ");
        scanf("%lf", &firstNumber);
    } else {
        printf("Enter the first operand: ");
        scanf("%lf", &firstNumber);

        printf("Enter the second operand: ");
        scanf("%lf", &secondNumber);
    }

    switch (operator)
    {
    case '+':
        printf("%.1lf + %.1lf = %.1lf\n", firstNumber, secondNumber, firstNumber + secondNumber);
        break;
    case '-':
        printf("%.1lf - %.1lf = %.1lf\n", firstNumber, secondNumber, firstNumber - secondNumber);    
        break;
    case '*':
        printf("%.1lf * %.1lf = %.1lf\n", firstNumber, secondNumber, firstNumber * secondNumber);
        break;
    case '/':
        if (secondNumber != 0.0)
        {
            printf("%.1lf / %.1lf = %.1lf\n", firstNumber, secondNumber, firstNumber / secondNumber);
        } else {
            printf("Division by zero is not allowed.\n");
        }
    case '^':
        printf("%.1lf ^ %.1lf = %.1lf\n", firstNumber, secondNumber, pow(firstNumber, secondNumber));
        break;
    case 'r':
        if (firstNumber >= 0)
        {
            printf("√%.1lf  = %.1lf\n", firstNumber, sqrt(firstNumber));
        } else {
            printf("Square root of a negative number is not allowed.\n");
        }
        break;
    default:
        printf("Error! Operator is not correct\n");
        break;
    }

    return 0;
}