% K2
X1 = [
    1.64 1.20 76.8 71.2 48.0 4.05 5.35 20.5 1.85;
    1.48 1.36 82.6 76.5 37.0 3.55 4.75 20.5 1.85;
    1.64 1.20 78.8 73.0 42.0 3.70 4.85 35.0 1.85;
    1.65 1.05 76.2 65.0 43.0 3.55 5.40 18.5 1.85;
    1.72 1.24 82.7 76.5 43.0 4.10 5.15 22.5 1.85;
    2.20 1.68 91.2 84.3 36.0 3.55 4.55 20.5 1.85;
    2.56 2.08 78.8 73.0 40.0 3.10 4.85 11.5 1.85;
    2.12 1.52 89.6 83.0 45.0 3.25 4.65 23.0 2.30;
    1.68 1.26 91.0 84.2 40.0 3.35 4.60 32.0 2.00;
];

% K1
X2 = [
    1.72 1.20 79.4 73.7 51.0 3.55 5.20 32.0 2.30;
    1.36 1.20 85.5 79.1 37.0 3.90 4.95 20.5 1.85;
    2.28 1.92 91.0 84.3 45.0 3.25 4.45 20.5 1.85;
    2.08 1.84 88.1 81.6 43.0 3.05 4.35 20.5 1.85;
    1.52 1.16 82.6 76.5 50.0 3.20 4.80 18.5 1.85;
    2.72 1.90 52.1 76.0 57.0 2.75 3.85 18.0 1.85;
    1.88 1.52 97.6 59.3 46.0 2.90 4.00 18.0 1.85;
    2.88 2.40 70.6 83.0 43.0 3.25 4.35 22.5 1.85;
    2.52 2.16 93.8 86.3 45.0 3.45 4.60 20.5 1.85;
    2.24 1.76 88.3 81.7 45.0 3.25 4.55 20.5 1.85;
    1.38 1.16 89.5 79.1 49.0 3.55 4.65 21.0 1.85;
    2.52 1.88 90.9 84.2 45.0 3.25 4.65 19.5 2.60;
    2.28 1.60 105 79.6 51.0 3.35 4.70 22.5 1.55;
    2.56 2.24 87.5 81.0 49.0 3.25 4.25 22.0 1.85;
    2.70 1.95 82.0 76.0 51.0 3.15 3.90 11.5 1.85;
];

% Data
X3 = [
    2.76 2.44 81.6 79.0 49.0 2.97 4.25 4.00 2.15;
    2.64 2.28 83.0 76.4 43.0 3.00 3.77 39.0 2.0;
    2.64 2.04 81.6 73.8 40.0 3.50 4.20 39.0 2.67;
    2.56 2.44 81.6 79.0 45.0 2.90 3.86 39.0 2.67;
    2.80 2.52 84.3 79.0 50.0 2.95 4.05 39.0 2.67;
    2.18 2.06 79.0 79.0 43.0 3.10 4.15 39.0 2.67;
    2.04 2.08 81.6 79.0 48.0 3.10 3.95 21.5 2.30;
    2.56 2.40 84.3 79.0 44.0 3.53 3.67 21.0 1.80;
    2.58 2.24 81.6 79.0 42.0 3.02 4.25 49.5 2.34;
    2.32 2.30 81.6 80.4 44.0 3.20 4.15 21.0 1.80;
    2.44 2.12 81.6 73.8 43.5 2.75 4.15 21.0 1.80;
    2.20 2.12 81.4 74.4 36.0 3.33 4.10 21.5 2.30;
    2.22 1.88 79.0 68.4 43.0 3.36 4.40 26.0 2.70;
    2.34 1.96 81.6 73.8 37.0 3.23 4.60 21.0 2.00;
    2.36 1.84 77.8 83.8 47.0 3.35 4.60 30.0 2.45;
    2.30 1.96 81.6 71.9 36.0 3.25 4.60 26.0 2.70;
    2.24 1.94 82.9 73.8 38.0 3.50 4.43 21.5 2.30;
    2.68 2.52 84.0 79.0 50.0 2.75 3.95 39.0 2.67;
    2.08 1.96 83.0 76.4 46.0 3.15 4.35 39.0 2.00;
];

% 2.3
M_K1 = mean(X2);
D_K1 = var(X2);
M_K2 = mean(X1);
D_K2 = var(X1);

fprintf("\n--- 2.3 K1 ---")
fprintf('\nОзнака | Математичне сподівання (M) | Дисперсія (D)\n');
fprintf('-------|----------------------------|--------------\n');
for i = 1:9
    fprintf('  X%d   |          %10.4f        |   %10.4f\n', i, M_K1(i), D_K1(i));
end

fprintf("\n--- 2.3 K2 ---")
fprintf('\nОзнака | Математичне сподівання (M) | Дисперсія (D)\n');
fprintf('-------|----------------------------|--------------\n');
for i = 1:9
    fprintf('  X%d   |          %10.4f        |   %10.4f\n', i, M_K2(i), D_K2(i));
end

% 2.4
M_total = mean([X1; X2]);
D_total = var([X1; X2]);
S_total = std([X1; X2]);

X1_norm = (X1 - M_total) ./ S_total;
X2_norm = (X2 - M_total) ./ S_total;
X3_norm = (X3 - M_total) ./ S_total;

fprintf('\n--- 2.4 Нормовані значення X1 ---\n');
disp(X1_norm());


% 2.5
X_train = [X1_norm; X2_norm];
n = size(X_train, 1);
R = zeros(n, n);

for j = 1:n
    for l = 1:n
        if j ~= l
            R(j,l) = sqrt(sum((X_train(j,:) - X_train(l,:)).^2));
        end
    end
end

fprintf('\n--- 2.5 Матриця відстаней R (фрагмент 5x5) ---\n');
disp(R(1:5, 1:5));


% 2.6
n1 = size(X2_norm, 1);
n2 = size(X1_norm, 1);

RA = R(n2+1:end, n2+1:end); 
RB = R(1:n2, 1:n2);

YA = exp(-cos(RA));
YB = exp(-sin(RB));

fprintf('\n--- 2.6 Потенціали класу K1 (фрагмент) ---\n');
disp(YA(1:5, 1:5));

fprintf('\n--- 2.6 Потенціали класу K2 (фрагмент) ---\n');
disp(YB(1:5, 1:5));


% 2.7
FA = sum(YA, 2);
FB = sum(YB, 2);
FAc = mean(FA);
FBc = mean(FB);

fprintf('\n--- 2.7 Сумарні потенціали FA (K1) ---\n');
disp(FA);
fprintf('Середній потенціал FAc: %.4f\n', FAc);

fprintf('\n--- 2.7 Сумарні потенціали FB (K2) ---\n');
disp(FB);
fprintf('Середній потенціал FBc: %.4f\n', FBc);

% 2.8
Decision_K1 = FA > FAc; 
Decision_K2 = FB > FBc;

fprintf('\n--- 2.8 Перевірка навчальної вибірки (K1) ---\n');
fprintf('Об''єкт | Сум. Пот. FA | Поріг FAc | Рішення\n');
for i = 1:length(FA)
    status = 'Правильно';
    if FA(i) <= FAc, status = 'Помилка'; end
    
    fprintf('  %2d   |   %7.4f   |  %8.4f  | %s\n', i, FA(i), FAc, status);
end

fprintf('\n--- 2.8 Перевірка навчальної вибірки (K2) ---\n');
fprintf('Об''єкт | Сум. Пот. FB | Поріг FBc | Рішення\n');
for i = 1:length(FB)
    status = 'Правильно';
    if FB(i) <= FBc, status = 'Помилка'; end

    fprintf('  %2d   |   %7.4f   |  %8.4f  | %s\n', i, FB(i), FBc, status);
end

% 2.9

fprintf('\n--- 2.9 Зведена таблиця (K1) ---\n');
fprintf(' № | X1-X9 (Ознаки)                               | FB      | FBc     | Клас | Рішення\n');
for i = 1:size(X2, 1)
    status = 'Правильно';
    if FA(i) <= FAc, status = 'Помилка'; end
    fprintf('%2d | ', i);
    fprintf('%4.1f ', X2(i, :));
    fprintf('| %7.4f | %7.4f | K1 | %s\n', FA(i), FAc, status);
end

fprintf('\n--- 2.9 Зведена таблиця (K2) ---\n');
fprintf(' № | X1-X9 (Ознаки)                               | FB      | FBc     | Клас | Рішення\n');
for i = 1:size(X1, 1)
    status = 'Правильно';
    if FB(i) <= FBc, status = 'Помилка'; end
    fprintf('%2d | ', i);
    fprintf('%4.1f ', X1(i, :));
    fprintf('| %7.4f | %7.4f | K2 | %s\n', FB(i), FBc, status);
end

% 2.10
n_K1 = size(X2, 1);
n_K2 = size(X1, 1);

n_rish_K1 = sum(FA > FAc); 
n_rish_K2 = sum(FB > FBc);

n_K1_err = sum(FA <= FAc);
n_K2_err = sum(FB <= FBc);

P_rish_K1 = n_rish_K1 / n_K1;
P_rish_K2 = n_rish_K2 / n_K2;
P_K2_rish_K1 = n_K2_err / n_K2;
P_K1_rish_K2 = n_K1_err / n_K1;
P_pom = (n_K1_err + n_K2_err) / (n_K1 + n_K2);

fprintf('\n--- 2.10 Кількісні показники навчання ---\n');
fprintf('Показник            | Значення\n');
fprintf('--------------------|----------\n');
fprintf('n (ріш K1)          |   %d\n', n_rish_K1);
fprintf('n (ріш K2)          |   %d\n', n_rish_K2);
fprintf('n (K2/ріш K1)       |   %d\n', n_K2_err);
fprintf('n (K1/ріш K2)       |   %d\n', n_K1_err);
fprintf('P (ріш K1)          |   %.4f\n', P_rish_K1);
fprintf('P (ріш K2)          |   %.4f\n', P_rish_K2);
fprintf('P (K2/ріш K1)       |   %.4f\n', P_K2_rish_K1);
fprintf('P (K1/ріш K2)       |   %.4f\n', P_K1_rish_K2);
fprintf('P пом               |   %.4f\n', P_pom);

figure;
bar([P_rish_K1, P_rish_K2, P_pom]);
set(gca, 'XTickLabel', {'P(ріш K1)', 'P(ріш K2)', 'P пом'});
title('Ймовірності результатів навчання');
ylabel('Значення ймовірності');
grid on;

% 2.11
thresholds = linspace(min([FA; FB]), max([FA; FB]), 100);
P_errors = zeros(size(thresholds));

for i = 1:length(thresholds)
    T = thresholds(i);
    err_K1 = sum(FA <= T);
    err_K2 = sum(FB <= T);
    P_errors(i) = (err_K1 + err_K2) / (n_K1 + n_K2);
end

[min_P, idx] = min(P_errors);
opt_T = thresholds(idx);

fprintf('\n--- 2.11 Вибір оптимального порога ---\n');
fprintf('Мінімальна імовірність помилки P_pom: %.4f\n', min_P);
fprintf('Оптимальне значення порога: %.4f\n', opt_T);

figure;
plot(thresholds, P_errors, 'LineWidth', 2);
hold on;
plot(opt_T, min_P, 'ro', 'MarkerSize', 10, 'LineWidth', 2);
title('Залежність імовірності помилки від значення порога');
xlabel('Поріг');
ylabel('P пом');
grid on;

% 2.12, Метод Монте-Карло
best_err = 1.0;

for k = 1:20000
    U_rand = rand(1, 9); U_rand = U_rand / sum(U_rand);
    alpha_rand = 1 + rand()*4;
    
    FA_t = zeros(n1, 1);
    for i = 1:n1
        for j = 1:n1
            if i ~= j
                d = sqrt(sum(U_rand .* (X2_norm(i,:) - X2_norm(j,:)).^2));
                FA_t(i) = FA_t(i) + exp(-alpha_rand * d);
            end
        end
    end
    
    FB_t = zeros(n2, 1);
    for i = 1:n2
        for j = 1:n1
            d = sqrt(sum(U_rand .* (X1_norm(i,:) - X2_norm(j,:)).^2));
            FB_t(i) = FB_t(i) + exp(-alpha_rand * d);
        end
    end

    T_rand = (mean(FA_t) + mean(FB_t)) / 2;
    
    current_err = (sum(FA_t <= T_rand) + sum(FB_t > T_rand)) / (n1 + n2);
    
    if current_err < best_err
        best_err = current_err;
        best_U = U_rand;
        best_T = T_rand;
        best_alpha = alpha_rand;

        FA_best = FA_t;
        FB_best = FB_t;
    end
end

fprintf('\n--- 2.12 Результати багатомірної оптимізації ---\n');
fprintf('Оптимальний коефіцієнт alpha: %.2f\n', best_alpha);
fprintf('Оптимальний поріг T: %.4f\n', best_T);
fprintf('Мінімальна помилка навчання: %.4f\n', best_err);
fprintf('Оптимальні ваги (U):\n');
disp(best_U');

% 2.13
n_miss_K1 = sum(FA_best <= best_T);  % K1 прийняті за K2
n_false_K2 = sum(FB_best > best_T);  % K2 прийняті за K1

P_miss = n_miss_K1 / n1;
P_false = n_false_K2 / n2;
total_P_opt = (n_miss_K1 + n_false_K2) / (n1 + n2);

fprintf('\n--- 2.13 Оцінка імовірності помилкових рішень ---\n');
fprintf('Кількість помилок K1 (пропуск): %d\n', n_miss_K1);
fprintf('Кількість помилок K2 (хибна тривога): %d\n', n_false_K2);
fprintf('Імовірність пропуску P(K2/K1): %.4f\n', P_miss);
fprintf('Імовірність хибної тривоги P(K1/K2): %.4f\n', P_false);
fprintf('Загальна помилка P_pom: %.4f\n', total_P_opt);

% 2.14
fprintf('\n--- 2.14 Результати класифікації X3 ---\n');
n3 = size(X3_norm, 1);
F3 = zeros(n3, 1);
for i = 1:n3
    Fi = 0;
    for j = 1:n1
        d = sqrt(sum(best_U .* (X3_norm(i,:) - X2_norm(j,:)).^2));
        Fi = Fi + exp(-best_alpha * d);
    end
    F3(i) = Fi;
    
    res = "K2 (Непридатний)";
    if Fi > best_T, res = "K1 (Придатний)"; end
    fprintf('%2d | F = %7.4f | %s\n', i, Fi, res);
end

figure;
stem(F3, 'filled'); hold on;
yline(best_T, 'r--', 'Поріг', 'LineWidth', 2);
title('Розподіл потенціалів X3');
grid on;