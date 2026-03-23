#include <stdio.h>
#include <stdlib.h>

struct client {
    int acc;
    char name[20];
    double bal;
};

void add(FILE *fp);
void view(FILE *fp);
void update(FILE *fp);
void del(FILE *fp);

int main() {
    FILE *fp = fopen("bank.dat", "rb+");
    int ch;

    if (!fp) {
        fp = fopen("bank.dat", "wb+");
    }

    do {
        printf("\n1.Add 2.View 3.Update 4.Delete 5.Exit\n");
        scanf("%d", &ch);

        switch (ch) {
            case 1: add(fp); break;
            case 2: view(fp); break;
            case 3: update(fp); break;
            case 4: del(fp); break;
        }
    } while (ch != 5);

    fclose(fp);
    return 0;
}

void add(FILE *fp) {
    struct client c;

    printf("Enter acc, name, balance: ");
    scanf("%d %s %lf", &c.acc, c.name, &c.bal);

    fseek(fp, (c.acc - 1) * sizeof(c), SEEK_SET);
    fwrite(&c, sizeof(c), 1, fp);
}

void view(FILE *fp) {
    struct client c;
    rewind(fp);

    printf("\nAcc\tName\tBalance\n");

    while (fread(&c, sizeof(c), 1, fp)) {
        if (c.acc != 0)
            printf("%d\t%s\t%.2f\n", c.acc, c.name, c.bal);
    }
}

void update(FILE *fp) {
    struct client c;
    int acc;
    double amt;

    printf("Enter acc: ");
    scanf("%d", &acc);

    fseek(fp, (acc - 1) * sizeof(c), SEEK_SET);
    fread(&c, sizeof(c), 1, fp);

    if (c.acc == 0) {
        printf("Not found\n");
        return;
    }

    printf("Enter amount (+/-): ");
    scanf("%lf", &amt);

    c.bal += amt;

    fseek(fp, -sizeof(c), SEEK_CUR);
    fwrite(&c, sizeof(c), 1, fp);
}

void del(FILE *fp) {
    struct client c = {0, "", 0};
    int acc;

    printf("Enter acc: ");
    scanf("%d", &acc);

    fseek(fp, (acc - 1) * sizeof(c), SEEK_SET);
    fwrite(&c, sizeof(c), 1, fp);
}
