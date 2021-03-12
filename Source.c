#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

// defininsanje cvora stabla
typedef struct treeNode {
    char info;
    struct treeNode* left;
    struct treeNode* right;
} TreeNode;

typedef struct stackNode {
    char info;
    struct stackNode* next;
} StackNode;

typedef struct stackNNode {
    TreeNode* info;
    struct stackNNode* next;
} StackNNode;

typedef struct clcStack {
    double info;
    struct clcStack* next;
}ClcStack;

typedef struct printList {
    int info;
    struct printList* next;
    struct printList* prev;
}PrintList;

void add_l(int val, PrintList** top, PrintList** bottom) {
    PrintList* new_node = (PrintList*)malloc(sizeof(PrintList));
    new_node->info = val;
    new_node->next = NULL;
    new_node->prev = NULL;
    if (*top != NULL) {
        new_node->next = *top;
        (*top)->prev = new_node;
    }
    else {
        new_node->next = NULL;
        *bottom = new_node;
    }
    *top = new_node;
}

int remove_l(PrintList** top, PrintList** bottom) {
    if (*top == NULL) return 0;
    PrintList* temp = *top;
    int data = (*top)->info;
    if ((*top)->next == NULL) *top = NULL, * bottom = NULL;
    else { (*top) = (*top)->next; (*top)->prev = NULL; }
    free(temp);
    return data;
}

void push_c(double val, ClcStack** top) {
    ClcStack* new_node = (ClcStack*)malloc(sizeof(ClcStack));
    new_node->info = val;
    new_node->next = NULL;
    if (*top != NULL) {
        new_node->next = *top;
    }
    *top = new_node;
}

double pop_c(ClcStack** top) {
    if (*top == NULL) return '\0';
    ClcStack* temp = *top;
    double data = (*top)->info;
    if ((*top)->next == NULL) *top = NULL;
    else (*top) = (*top)->next;
    free(temp);
    return data;
}

TreeNode* make_node(char val) {
    TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
    new_node->info = val;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}
//definicija funkcija steka
void push_op(char val, StackNode** top) {
    StackNode* new_node = (StackNode*)malloc(sizeof(StackNode));
    new_node->info = val;
    new_node->next = NULL;
    if (*top != NULL) {
        new_node->next = *top;
    }
    *top = new_node;
}

char pop_op(StackNode** top) {
    if (*top == NULL) return '\0';
    StackNode* temp = *top;
    char data = (*top)->info;
    if ((*top)->next == NULL) *top = NULL;
    else (*top) = (*top)->next;
    free(temp);
    return data;
}

char peek_op(StackNode* top) { return (top == NULL) ? NULL : top->info; }

void push_nd(TreeNode* node, StackNNode** top) {
    StackNNode* new_node = (StackNNode*)malloc(sizeof(StackNNode));
    new_node->info = node;
    if (*top != NULL) new_node->next = *top;
    else new_node->next = NULL;
    *top = new_node;
}

TreeNode* pop_nd(StackNNode** top) {
    if (*top == NULL) return '\0';
    StackNNode* temp = *top;
    TreeNode* data = (*top)->info;
    if ((*top)->next == NULL) *top = NULL;
    else *top = (*top)->next;
    free(temp);
    return data;
}

void update_node_stk(char val, StackNNode** top_nd) {
    TreeNode* new_node = make_node(val);
    if (!(val == 't' || val == 'l' || val == 'c' || val == '#')) new_node->right = pop_nd(top_nd);
    new_node->left = pop_nd(top_nd);
    push_nd(new_node, top_nd);

}

char* input_exp() {
    char ch;
    char* exp = (char*)malloc(100 * sizeof(char));
    int lenght = 0;
    printf("Unesite izraz: ");
    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (ch != ' ') {
            exp[lenght] = ch;
            ++lenght;
        }
    }
    exp[lenght] = 0;
    return exp;
}

int exp_to_tree(TreeNode** root, char* exp, double* values, char* chars) {
    char arr[100];
    memcpy(arr, exp, 100 * sizeof(char));
    int noo = 10;
    int count = 0;
    static char iprs[10][2] = { {'+', 2}, {'-', 2}, {'*', 3}, {'/', 3}, {'^', 5}, {'(', 8}, {'t', 7}, {'l', 7}, {'m', 7},{'#', 7} };
    static char sprs[10][2] = { {'+', 2}, {'-', 2}, {'*', 3}, {'/', 3}, {'^', 4}, {'(', 0}, {'t', 6}, {'l', 6}, {'m', 6},{'#', 7} };
    StackNode* top_op = NULL;
    StackNNode* top_nd = NULL;
    for (int i = 0; arr[i] != '\0'; i++) {
        if (isupper(arr[i])) {
            int flag = 1;
            TreeNode* node = make_node(arr[i]);
            int j;
            for (j = 0; j < count;j++) if (chars[j] == arr[i]) { flag = 0; break; };
            if (flag) {
                chars[count] = arr[i];
                count++;
            }
            (double*)(node->right) = &(values[j]);
            push_nd(node, &top_nd);

        }
        else if (arr[i] >= 0 && arr[i] <= 9) {
            if (!(arr[i + 1] >= 0 && arr[i + 1] <= 9)) {
                TreeNode* node = make_node(arr[i]);
                push_nd(node, &top_nd);
            }
        }
        else if (arr[i] == '(' && arr[i + 1] == '-') {
            arr[i + 1] = '#';
            push_op('(', &top_op);
        }
        else if (arr[i] == ')' || arr[i] == ',') {
            char op;
            while ((op = pop_op(&top_op)) != '(') {
                update_node_stk(op, &top_nd);
            }
            if (arr[i] == ',') push_op('(', &top_op);
        }
        else {
            int ipr = 0;
            int spr = 0;
            char top = peek_op(top_op);
            if (top == NULL) { push_op(arr[i], &top_op); continue; }
            else {
                for (int j = 0; j < noo; j++) ipr = (iprs[j][0] == arr[i]) ? iprs[j][1] : ipr;
                do {
                    for (int j = 0; j < noo; j++) spr = (sprs[j][0] == top) ? sprs[j][1] : spr;
                    if (ipr > spr) { push_op(arr[i], &top_op); break; }
                    else {
                        update_node_stk(pop_op(&top_op), &top_nd);
                    }
                    top = peek_op(top_op);
                    if (top == NULL) { push_op(arr[i], &top_op); spr = 0; }
                } while (ipr <= spr);
                if (arr[i] == 'l' || arr[i] == 't') i++;
                if (arr[i] == 'm' || arr[i] == 'c') i += 2;
            }


        }
    }
    while (top_op != NULL) {
        update_node_stk(pop_op(&top_op), &top_nd);
    }
    *root = pop_nd(&top_nd);

    return count;
}

double* val_input(char* chars, double* values, int size) {
    for (int i = 0; i < size; i++) {
        printf("vrednost promenjive %c: ", chars[i]);
        scanf("%lf%*c", &values[i]);
    }
    return values;
}

void print_postfix(TreeNode* root) {
    StackNode* top_op = NULL;
    StackNNode* top_nd = NULL;
    TreeNode* node;
    push_nd(root, &top_nd);
    do {
        node = pop_nd(&top_nd);
        push_op(node->info, &top_op);
        if (node->left != NULL) push_nd(node->left, &top_nd); else continue;
        if (node->right != NULL)push_nd(node->right, &top_nd);
    } while (top_nd != NULL);
    while (top_op != NULL) {
        char ch = pop_op(&top_op);
        if (ch == 't') printf("%c%c", 't', 'g');
        else if (ch == 'l')  printf("%c%c", 'l', 'n');
        else if (ch == 'm') printf("%c%c%c", 'm', 'i', 'n');
        else if (ch == 'c') printf("%c%c%c", 'c', 'o', 's');
        else printf("%c", ch);
    }
}

void clc(char op, ClcStack** top) {
    double val1 = pop_c(top);
    double res = 0;
    if (!(op == 't' || op == 'l' || op == 'c' || op == '#')) {
        double val2 = pop_c(top);
        switch (op)
        {
        case '+':
            res = val2 + val1;
            break;
        case '-':
            res = val2 - val1;
            break;
        case '*':
            res = val2 * val1;
            break;
        case '/':
            res = val2 / val1;
            break;
        case '^':
            res = pow(val2, val1);
            break;
        case 'm':
            res = (val1 > val2) ? val2 : val1;
            break;
        }
    }
    else {
        switch (op)
        {
        case '#':
            res = val1 * (-1);
            break;
        case 'l':
            res = log(val1);
            break;
        case 't':
            res = tan(val1);
            break;
        case 'c':
            res = cos(val1);
            break;
        }
    }
    push_c(res, top);
}

double calculation(TreeNode* root) {
    StackNNode* top_nd = NULL;
    ClcStack* top_c = NULL;
    TreeNode* cur = root;
    TreeNode* tmp;
    char op;
    do {
        while (cur != NULL) {
            if (cur->right && cur->left) push_nd(cur->right, &top_nd);
            push_nd(cur, &top_nd);
            cur = cur->left;
        }
        cur = pop_nd(&top_nd);
        if (cur != NULL && cur->left && cur->right && top_nd != NULL && cur->right == top_nd->info) {
            tmp = pop_nd(&top_nd);
            push_nd(cur, &top_nd);
            cur = tmp;
        }
        else if(cur!=NULL) {
            op = cur->info;
            if (isupper(op)) push_c(*((double*)(cur->right)), &top_c);
            else if (op >= '0' && op <= '9') push_c((double)(op - '0'), &top_c);
            else clc(op, &top_c);
            cur = NULL;
        }
    } while (top_nd != NULL);
    return pop_c(&top_c);
}

void print_tree(TreeNode* root) {
    StackNNode* top = NULL;
    PrintList* lvl = NULL;
    PrintList* end = NULL;
    TreeNode* node = NULL;
    push_nd(root, &top);
    add_l(1, &lvl, &end);
    int nlvl;
    do {
        node = pop_nd(&top);
        nlvl = remove_l(&lvl, &end);
        PrintList* cur = end;
        for (int i = 1; i <= nlvl; i++) {
            if (cur == NULL) (i == nlvl) ? printf("'-- ") : printf("    ");
            else {
                if (i == nlvl) {
                    if (cur->info == nlvl) printf("|-- ");
                    else  printf("'-- ");
                }
                else if (i == cur->info) {
                    printf("|   ");
                    cur = cur->prev;
                }
                else printf("    ");
            }

        }
        if (node->info == 't') printf("tg\n");
        else if (node->info == 'l') printf("ln\n");
        else if (node->info == 'm') printf("min\n");
        else if (node->info == 'c') printf("cos\n");
        else printf("%c\n", node->info);

        if (node->left) {
            if (node->right) push_nd(node->right, &top);
            else push_nd(make_node(' '), &top);
            push_nd(node->left, &top);
            nlvl++;
            add_l(nlvl, &lvl, &end);
            add_l(nlvl, &lvl, &end);
        }

    } while (top != NULL);
}

//vraca 1 ako je promenjiva po kojoj se diferencira u podstablu
int chk_var(char variable, TreeNode* root) {

    StackNNode* top = NULL;
    TreeNode* node = NULL;
    push_nd(root, &top);
    do {
        node = pop_nd(&top);
        if (node->info == variable) return 1;
        if (node->left != NULL) {
            if (node->right != NULL) push_nd(node->right, &top);
            push_nd(node->left, &top);
        }
    } while (top != NULL);
    return 0;
}

TreeNode* copy_tree(TreeNode* root, TreeNode** copyr) {
    StackNNode* top_post = NULL;
    StackNNode* top_sort = NULL;
    TreeNode* node;
    TreeNode* new_node;
    push_nd(root, &top_sort);
    do {
        node = pop_nd(&top_sort);
        push_nd(node, &top_post);
        if (node->left != NULL) push_nd(node->left, &top_sort);
        if (node->right != NULL && !(isupper(node->info)))push_nd(node->right, &top_sort);
    } while (top_sort != NULL);
    while (top_post != NULL) {
        node = pop_nd(&top_post);
        new_node = make_node(node->info);
        if (isupper(node->info)) {
            new_node->right = node->right;
        }
        else if (isdigit(node->info));
        else {
            if (!(new_node->info == 't' || new_node->info == 'l' || new_node->info == 'c' || new_node->info == '#')) new_node->right = pop_nd(&top_sort);
            new_node->left = pop_nd(&top_sort);
        }
        push_nd(new_node, &top_sort);
    }
    *copyr = pop_nd(&top_sort);
    return *copyr;
}

TreeNode* diff(TreeNode* node, char var) {
    if (node->info == var) return make_node('1');
    else return make_node('0');
}

void derivative(TreeNode* root, TreeNode** derroot, char variable) {
    StackNNode* top_d = NULL;
    StackNNode* top_h = NULL;
    StackNNode* top_o = NULL;
    TreeNode* node;
    push_nd(root, &top_h);
    do {
        node = pop_nd(&top_h);
        push_nd(node, &top_d);
        if (node->left != NULL) push_nd(node->left, &top_h); else continue;
        if (node->right != NULL)push_nd(node->right, &top_h);
    } while (top_h != NULL);
    while (top_d != NULL) {
        node = pop_nd(&top_d);
        if (isupper(node->info)) {
            push_nd(node, &top_h);
            push_nd(node, &top_o);
        }
        else {
            TreeNode* new_node = NULL;
            TreeNode* new_node2 = NULL;
            TreeNode* lv = NULL;
            TreeNode* rv = NULL;
            TreeNode* olv = NULL;
            TreeNode* orv = NULL;
            switch (node->info)
            {
            case '+':                                                   //radi!!!
                pop_nd(&top_o);
                pop_nd(&top_o);
                push_nd(node, &top_o);
                rv = pop_nd(&top_h);
                lv = pop_nd(&top_h);
                if (isupper(lv->info)) lv = diff(lv, variable);
                if (isupper(rv->info)) rv = diff(rv, variable);
                new_node = make_node('+');
                new_node->left = lv;
                new_node->right = rv;
                push_nd(new_node, &top_h);
                break;

            case '-':
                pop_nd(&top_o);
                pop_nd(&top_o);
                push_nd(node, &top_o);
                rv = pop_nd(&top_h);
                lv = pop_nd(&top_h);
                if (isupper(lv->info)) lv = diff(lv, variable);
                if (isupper(rv->info)) rv = diff(rv, variable);
                new_node = make_node('-');
                new_node->left = lv;
                new_node->right = rv;
                push_nd(new_node, &top_h);
                break;

            case '*':                                               //radii
                new_node = make_node('+');
                new_node->left = make_node('*');
                new_node->right = make_node('*');
                (new_node->right)->left = copy_tree(pop_nd(&top_o), make_node(top_o->info));
                (new_node->left)->left = copy_tree(pop_nd(&top_o), make_node(top_o->info));
                rv = pop_nd(&top_h);
                lv = pop_nd(&top_h);
                if (isupper(lv->info)) lv = diff(lv, variable);
                if (isupper(rv->info)) rv = diff(rv, variable);
                (new_node->right)->right = lv;
                (new_node->left)->right = rv;
                push_nd(node, &top_o);
                push_nd(new_node, &top_h);
                break;

            case '/':
                orv = pop_nd(&top_o);
                olv = pop_nd(&top_o);
                new_node = make_node('/');
                new_node->left = make_node('-');
                new_node->right = make_node('^');
                (new_node->right)->left = copy_tree(orv, make_node(orv->info));
                (new_node->right)->right = make_node('2');
                (new_node->left)->left = make_node('*');
                (new_node->left)->right = make_node('*');
                ((new_node->left)->left)->right = copy_tree(orv, make_node(orv->info));
                rv = pop_nd(&top_h);
                lv = pop_nd(&top_h);
                if (isupper(lv->info)) lv = diff(lv, variable);
                if (isupper(rv->info)) rv = diff(rv, variable);
                ((new_node->left)->left)->left = lv;
                ((new_node->left)->right)->left = rv;
                ((new_node->left)->right)->right = copy_tree(olv, make_node(olv->info));
                push_nd(node, &top_o);
                push_nd(new_node, &top_h);
                break;

            case '^':
                rv = pop_nd(&top_h);
                lv = pop_nd(&top_h);
                if (isupper(lv->info)) lv = diff(lv, variable);
                if (isupper(rv->info)) rv = diff(rv, variable);
                orv = pop_nd(&top_o);
                olv = pop_nd(&top_o);
                int var_rv = chk_var(variable, orv);
                int var_lv = chk_var(variable, olv);
                if (var_rv && var_lv) { printf("nedefinisano diferenciranje\n"); *derroot = NULL; return; }
                if (var_lv) {
                    new_node = make_node('^');
                    new_node->left = lv;
                    new_node->right = make_node('-');
                    (new_node->right)->left = copy_tree(orv, make_node(orv->info));
                    (new_node->right)->right = make_node('1');
                    push_nd(node, &top_o);
                    if (!isupper(lv->info)) {
                        new_node2 = make_node('*');
                        new_node2->left = new_node;
                        new_node2->right = lv;
                        push_nd(new_node2, &top_h);
                    }
                    else push_nd(new_node, &top_h);
                }
                else if (var_rv) {
                    new_node = make_node('*');
                    new_node->left = copy_tree(node, make_node(node->info));
                    new_node->right = make_node('l');
                    (new_node->right)->left = lv;
                    push_nd(node, &top_o);
                    if (!isupper(rv->info)) {
                        new_node2 = make_node('*');
                        new_node2->left = new_node;
                        new_node2->right = rv;
                        push_nd(new_node2, &top_h);
                    }
                    else push_nd(new_node, &top_h);
                }
                else {
                    push_nd(make_node('0'), &top_h);
                    push_nd(node, &top_o);

                }
                break;

            case 'l':
                lv = pop_nd(&top_h);
                //if (isupper(lv->info)) lv = diff(lv, variable);
                if (!chk_var(variable, lv)) push_nd(make_node('0'), &top_h);
                else {
                    new_node = make_node('/');
                    new_node->left = make_node('1');
                    new_node->right = copy_tree(pop_nd(&top_o), make_node(top_o->info));
                    push_nd(node, &top_o);
                    if (!isupper(lv->info) && chk_var(variable, lv)) {
                        new_node2 = make_node('*');
                        new_node2->left = new_node;
                        new_node2->right = lv;
                        push_nd(new_node2, &top_h);
                    }
                    else {
                        //push_nd(make_node('0'), &top_h);
                        //push_nd(node, &top_o);
                        push_nd(new_node, &top_h);
                    }
                }
                break;

            case 't':
                lv = pop_nd(&top_h);
                //if (isupper(lv->info)) lv = diff(lv, variable);
                if (chk_var(variable, lv)) {
                    new_node = make_node('/');
                    new_node->left = make_node('1');
                    new_node->right = make_node('^');
                    (new_node->right)->left = make_node('c');
                    (new_node->right)->right = make_node('2');
                    ((new_node->right)->left)->left = copy_tree(pop_nd(&top_o), make_node(top_o->info));
                    push_nd(node, &top_o);
                    if (!isupper(lv->info)) {
                        new_node2 = make_node('*');
                        new_node2->left = new_node;
                        new_node2->right = lv;
                        push_nd(new_node2, &top_h);
                    }
                    else push_nd(new_node, &top_h);

                }
                else {
                    push_nd(make_node('0'), &top_h);
                    push_nd(node, &top_o);
                }
                break;

            case 'm':
                rv = pop_nd(&top_h);
                lv = pop_nd(&top_h);
                if (isupper(lv->info)) lv = diff(lv, variable);
                if (isupper(rv->info)) rv = diff(rv, variable);
                orv = pop_nd(&top_o);
                olv = pop_nd(&top_o);
                if (chk_var(variable, olv) || chk_var(variable, orv)) {
                    new_node = make_node('?');
                    new_node->left = make_node('o');
                    new_node->left->left = copy_tree(olv, make_node(olv->info)); // ne kopira lepo??
                    new_node->left->right = copy_tree(orv, make_node(orv->info));
                    new_node->right = make_node('n');
                    new_node->right->left = lv;
                    new_node->right->right = rv;
                    push_nd(new_node, &top_h);
                    push_nd(node, &top_o);
                }
                else {
                    push_nd(make_node('0'), &top_h);
                    push_nd(node, &top_o);
                }
                break;

            }
        }
    }
    *derroot = pop_nd(&top_h);
}

TreeNode* remove_min(TreeNode** node_next) {
    TreeNode* tmp;
    tmp = (*node_next)->right;
    if (calculation(((*node_next)->left)->left) >= calculation((*node_next)->left->right)) {
        //del_tree((*node_next)->left);
        (*node_next)->left = tmp->left;
        //del_tree(tmp->right);
    }
    else {
        //del_tree((*node_next)->left);
        (*node_next)->left = tmp->right;
        //del_tree(tmp->left);
    }
    free(tmp);
    return (*node_next)->left;
}
TreeNode* decide_tree(TreeNode* derroot) {
    StackNNode* top = NULL;
    TreeNode* node;
    TreeNode* new_root = copy_tree(derroot, make_node(derroot->info));
    push_nd(new_root, &top);
    do {
        node = pop_nd(&top);
        if (node->info != '\0' && node->left != NULL && (node->left)->info == '?') {
            node->left = remove_min(&(node->left));

        }
        if (node->info != '\0' && node->right != NULL && (node->right)->info == '?' && node->info != '\0') {
            node->right = remove_min(&(node->right));
        }
        if (node->info != '\0' && node->left != NULL) push_nd(node->left, &top);
        if (node->info != '\0' && node->right != NULL)push_nd(node->right, &top);

    } while (top != NULL);
    return new_root;
}

int menu() {
    int choice;
    printf("\n");
    printf("1. Unosenje izraza \n2. Unosenje vrednosti promenjivih \n3. Ispis stabla \n4. Ispis izraza u postfix notaciji \n5. Racunanje izraza \n6. Diferenciranje izraza \n7. Kraj\n");
    printf("Unesite broj stavke: ");
    scanf("%d%*c", &choice);
    return choice;
}

int main() {
    TreeNode* root = NULL;
    TreeNode* derroot = NULL;
    TreeNode* derroot_cur = NULL;
    int choice = 1;
    char* exp = NULL;
    int values_set = 0;
    int min_in_exp;
    int No_variables = 0;
    char var;
    int n;
    char* chs = (char*)malloc(26 * sizeof(char));
    double* vals = (double*)malloc(26 * sizeof(double));
    while (choice != 9) {
        switch (menu())
        {
        case 1:
            min_in_exp = 0;
            exp = input_exp();
            printf("Uneti izraz je: %s\n", exp);
            No_variables = exp_to_tree(&root, exp, vals, chs);
            for (int i = 0; exp[i] != '\0'; i++) if (exp[i] == 'm') min_in_exp = 1;
            break;
        case 2:
            if (No_variables) vals = val_input(chs, vals, No_variables);
            else break;
            if (min_in_exp && derroot) {
                derroot_cur = decide_tree(derroot);
            }
            values_set = 1;
            break;
        case 3:
            printf("Unesite 0 za ispis stabla izraza ili 1 za ispis stabla prvog izvoda: ");
            scanf("%d%*c", &n);
            if (n == 0) print_tree(root);
            else if (n == 1 && min_in_exp) print_tree(derroot_cur);
            else if (n == 1) print_tree(derroot);
            break;
        case 4:
            print_postfix(root);
            printf("\n");
            //printf("%lf",calculation(root));
            break;
        case 5:
            printf("Unesite 0 za izracunavanje izraza ili 1 za izracunavanje prvog izvoda: ");
            scanf("%d%*c", &n);
            if (n == 0) printf("%f", calculation(root));
            else if (n == 1 && min_in_exp) printf("%f", calculation(derroot_cur));
            else if (n == 1) printf("%f", calculation(derroot));
            break;
        case 6:
            printf("promenjiv po kojoj se diferencira: ");
            scanf("%c%*c", &var);
            derivative(root, &derroot, var);
            print_tree(derroot);
            if (min_in_exp && values_set) {
                derroot_cur = decide_tree(derroot);
            }
            break;
        case 7:
            exit(0);
            break;
        }

    }
}

//A*B+min(A,A*B)
//A+C-ln(B)+tg(A)
//B+C-min(ln(A),-tg(A))
//B+tg(A)-min(ln(A),A)