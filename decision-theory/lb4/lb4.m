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


% 2.3 K1
M_K1 = mean(X2);
D_K1 = var(X2);

fprintf("\n--- 2.3 ---")
fprintf('\nОзнака | Математичне сподівання (M) | Дисперсія (D)\n');
fprintf('-------|----------------------------|--------------\n');
for i = 1:9
    fprintf('  X%d   |          %10.4f        |   %10.4f\n', i, M_K1(i), D_K1(i));
end

% 2.4 K2
M_K2 = mean(X1);
D_K2 = var(X1);

fprintf("\n--- 2.4 ---")
fprintf('\nОзнака | Математичне сподівання (M) | Дисперсія (D)\n');
fprintf('-------|----------------------------|--------------\n');
for i = 1:9
    fprintf('  X%d   |          %10.4f        |   %10.4f\n', i, M_K2(i), D_K2(i));
end

% 2.5
R1 = corrcoef(X2);
R2 = corrcoef(X1);

fprintf("\n--- 2.5 ---\n")
disp('Матриця кореляції R1 (Клас К1):');
disp(R1);

disp('Матриця кореляції R2 (Клас К2):');
disp(R2);

% 2.6
[R1, P1] = corrcoef(X2);
[R2, P2] = corrcoef(X1);

R1_sig = R1;
R1_sig(P1 > 0.05) = 0;

R2_sig = R2;
R2_sig(P2 > 0.05) = 0;

fprintf("\n--- 2.6 ---\n")

disp('Значущі коефіцієнти кореляції для К1:');
disp(R1_sig);

disp('Значущі коефіцієнти кореляції для К2:');
disp(R2_sig);

% 2.7
S1 = cov(X2);
S2 = cov(X1);
b = (S1 + S2) \ (M_K1 - M_K2)';

M_G_K1 = M_K1 * b;
M_G_K2 = M_K2 * b;

fprintf("--- 2.7 ---\n")
fprintf('M*[G/K1] = %10.4f\n', M_G_K1);
fprintf('M*[G/K2] = %10.4f\n', M_G_K2);

% 2.8
D_G_K1 = b' * S1 * b;
D_G_K2 = b' * S2 * b;

fprintf("\n--- 2.8 ---\n")
fprintf('D*[G/K1] = %10.4f\n', D_G_K1);
fprintf('D*[G/K2] = %10.4f\n', D_G_K2);

% 2.9

fprintf("\n--- 2.9 ---\n")

disp('Оптимальні коефіцієнти дискримінантної функції (вектор b):')
for i = 1:9
    fprintf('b%d = %10.4f\n', i, b(i));
end

% 2.10
G1 = X2 * b;
G2 = X1 * b;

steps = linspace(min([M_G_K1, M_G_K2]), max([M_G_K1, M_G_K2]), 100);
errors = zeros(size(steps));

for i = 1:length(steps)
    threshold = steps(i);
    err1 = sum(G1 < threshold); 
    err2 = sum(G2 > threshold);
    errors(i) = (err1 + err2) / (length(G1) + length(G2));
end

[min_err, idx] = min(errors);
Pg_opt = steps(idx);

fprintf("\n--- 2.10 ---\n")
fprintf('Оптимальний поріг Pg = %10.4f\n', Pg_opt);
fprintf('Мінімальна ймовірність помилки = %10.4f\n', min_err);

% 2.11
G1_study = X2 * b;
G2_study = X1 * b;

fprintf("\n--- 2.11 ---\n")
disp('Значення G для навчальної вибірки К1:')
disp(G1_study')

disp('Значення G для навчальної вибірки К2:')
disp(G2_study')

% 2.12
f_error = @(p) (sum(G1_study < p) + sum(G2_study > p)) / (length(G1_study) + length(G2_study));

options = optimset('Display','off');
Pg_final = fminbnd(f_error, min(mean(G2_study), mean(G1_study)), max(mean(G2_study), mean(G1_study)), options);

fprintf("\n--- 2.12 ---\n")
fprintf('Оптимальний поріг після уточнення: %10.4f\n', Pg_final);
fprintf('Мінімальна ймовірність помилки: %10.4f\n', f_error(Pg_final));

% 2.13
M1 = mean(G1_study);
M2 = mean(G2_study);

if M1 > M2
    res_K1 = G1_study >= Pg_final;
    res_K2 = G2_study < Pg_final;
else
    res_K1 = G1_study < Pg_final;
    res_K2 = G2_study >= Pg_final;
end

fprintf("\n--- 2.13 ---\n")
fprintf('Точність для класу К1: %.2f%%\n', sum(res_K1)/length(res_K1)*100);
fprintf('Точність для класу К2: %.2f%%\n', sum(res_K2)/length(res_K2)*100);

% 2.14
n1 = length(G1_study);
n2 = length(G2_study);

err_K1 = sum(~res_K1);
err_K2 = sum(~res_K2);

P_err_K1 = err_K1 / n1;
P_err_K2 = err_K2 / n2;
P_total = (err_K1 + err_K2) / (n1 + n2);

fprintf("\n--- 2.14 ---\n")
fprintf('Оптимальний поріг Pg = %.4f\n', Pg_final);
fprintf('Помилка для класу К1 (P1): %.4f\n', P_err_K1);
fprintf('Помилка для класу К2 (P2): %.4f\n', P_err_K2);
fprintf('Загальна ймовірність помилки: %.4f\n', P_total);

% 2.15. За ваговими коефіцієнтами
[sorted_b, idx] = sort(abs(b), 'descend');

fprintf("\n--- 2.15 ---\n")
fprintf('Рейтинг інформативності ознак (від найбільш до найменш важливої):\n');
for i = 1:length(idx)
    fprintf('%d місце: Ознака X%d (вага |b| = %.4f)\n', i, idx(i), sorted_b(i));
end

top_features = idx(1:6);
fprintf('\nРекомендовано залишити ознаки: %s\n', strjoin(string(top_features), ', '));

% 2.16
idx_top = [2, 1, 7, 9, 6, 5]; 
XK1_red = X2(:, idx_top);
XK2_red = X1(:, idx_top);

mk1_red = mean(XK1_red)';
mk2_red = mean(XK2_red)';
sk1_red = cov(XK1_red);
sk2_red = cov(XK2_red);

b_red = (sk1_red + sk2_red) \ (mk1_red - mk2_red);

g1_red = XK1_red * b_red;
g2_red = XK2_red * b_red;

f_err_final = @(p) (sum(g1_red < p) + sum(g2_red > p)) / (length(g1_red) + length(g2_red));
pg_red = fminbnd(f_err_final, min(mean(g1_red), mean(g2_red)), max(mean(g1_red), mean(g2_red)));

fprintf("\n--- 2.16 ---\n")
fprintf('Новий оптимальний поріг Pg: %10.4f\n', pg_red);
fprintf('Ймовірність помилки на 6 ознаках: %10.4f\n', f_err_final(pg_red));

% 2.18
X3_red = X3(:, [2, 1, 7, 9, 6, 5]); 
G_test = X3_red * b_red;

fprintf('\n--- 2.18 ---\n')
for i = 1:length(G_test)
    if G_test(i) >= pg_red
        cl = 1;
    else
        cl = 2;
    end
    fprintf('Прилад %d: G = %.4f -> Клас K%d\n', i, G_test(i), cl);
end