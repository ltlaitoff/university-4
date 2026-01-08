Mx = 4.0;
My = 20.0;
sig_x = 0.6;
sig_y = 1.2;
Dx = sig_x^2;
Dy = sig_y^2;

x1 = 5.0;
x2 = 4.0;
x3 = 3.0;
x4 = 2.0;
X_values = [x1, x2, x3, x4];

r = -0.9;
Y_edge = My - sig_x;


% 2.3
z = (Y_edge - My) / sig_y;
P_K1 = normcdf(z);
P_K2 = 1 - P_K1;

fprintf('--- 2.3 ---\n');
fprintf('Нормована межа z = %.4f\n', z);
fprintf('Апріорна ймовірність P(K1) = %.4f\n', P_K1);
fprintf('Апріорна ймовірність P(K2) = %.4f\n', P_K2);

% 2.4
W_z = normpdf(z); 
Mx_K1 = Mx + r * sig_x * (W_z / P_K1);
Mx_K2 = Mx - r * sig_x * (W_z / P_K2);

fprintf('\n--- 2.4 ---\n');
fprintf('Значення густини W(z) = %.4f\n', W_z);
fprintf('M[x/K1] (середній шум придатних) = %.4f\n', Mx_K1);
fprintf('M[x/K2] (середній шум дефектних) = %.4f\n', Mx_K2);

% 2.5
k = [0, 0.2, -0.2, 0.4, -0.4, 0.6, -0.6, 0.8, -0.8, 1, -1, 2, -2, 3, -3];
k = sort(k);

x_pts = Mx + k * sig_x;
Wx = normpdf(x_pts, Mx, sig_x);

fprintf('\n--- 2.5: Таблиця безумовної густини W(x) ---\n');
fprintf('%10s | %10s | %10s\n', 'Відхилення', 'x', 'W(x)');
fprintf('-------------------------------------------\n');
for i = 1:length(x_pts)
    fprintf('%10.1fs | %10.4f | %10.4f\n', k(i), x_pts(i), Wx(i));
end

% 2.6
term_K1 = (W_z / P_K1) * (z + (W_z / P_K1));
term_K2 = (W_z / P_K2) * (z - (W_z / P_K2));

Dx_K1 = Dx * (1 - r^2 * term_K1);
Dx_K2 = Dx * (1 + r^2 * term_K2);

sig_x_K1 = sqrt(Dx_K1);
sig_x_K2 = sqrt(Dx_K2);

k_vec = [-3, -2, -1, -0.8, -0.6, -0.4, -0.2, 0, 0.2, 0.4, 0.6, 0.8, 1, 2, 3];

x_K1 = Mx_K1 + k_vec * sig_x;
W_x_K1 = normpdf(x_K1, Mx_K1, sig_x_K1);

x_K2 = Mx_K2 + k_vec * sig_x;
W_x_K2 = normpdf(x_K2, Mx_K2, sig_x_K2);

fprintf('\n--- 2.6: Умовні густини W(x/K1) та W(x/K2) ---\n');
fprintf('%5s | %10s | %10s | %10s | %10s\n', 'k', 'x(K1)', 'W(x/K1)', 'x(K2)', 'W(x/K2)');
fprintf('-------------------------------------------------------------\n');
for i = 1:length(k_vec)
    fprintf('%5.1f | %10.4f | %10.4f | %10.4f | %10.4f\n', ...
        k_vec(i), x_K1(i), W_x_K1(i), x_K2(i), W_x_K2(i));
end

% 2.7

f_opt = @(x) (P_K1 * normpdf(x, Mx_K1, sig_x_K1)) - (P_K2 * normpdf(x, Mx_K2, sig_x_K2));
X_kl = fzero(f_opt, Mx);

fprintf('\n--- 2.7: Порогове значення X_kl ---\n');
fprintf('Оптимальний поріг класифікації X_kl = %.4f\n', X_kl);

% 2.8
P_rish_K1 = 1 - normcdf(X_kl, Mx, sig_x);
P_rish_K2 = normcdf(X_kl, Mx, sig_x);

fprintf('\n--- 2.8 ---\n');
fprintf('P(ріш К1) = %.4f\n', P_rish_K1);
fprintf('P(ріш К2) = %.4f\n', P_rish_K2);

% 2.9
z_kl = (X_kl - Mx) / sig_x;
W_z_kl = normpdf(z_kl);

Mx_rish_K1 = Mx + sig_x * (W_z_kl / (1 - normcdf(z_kl)));
Mx_rish_K2 = Mx - sig_x * (W_z_kl / normcdf(z_kl));

My_rish_K1 = My + r * (sig_y / sig_x) * (Mx_rish_K1 - Mx);
My_rish_K2 = My + r * (sig_y / sig_x) * (Mx_rish_K2 - Mx);

fprintf('\n--- 2.9 ---\n');
fprintf('M[y / ріш K1] (середня якість обраних К1) = %.4f\n', My_rish_K1);
fprintf('M[y / ріш K2] (середня якість обраних К2) = %.4f\n', My_rish_K2);

% 2.10

k_y = [0, 0.2, -0.2, 0.4, -0.4, 0.6, -0.6, 0.8, -0.8, 1, -1, 2, -2, 3, -3];
k_y = sort(k_y);

y_pts = My + k_y * sig_y;
Wy = normpdf(y_pts, My, sig_y);

fprintf('\n--- 2.10: Таблиця безумовної густини W(y) ---\n');
fprintf('%10s | %10s | %10s\n', 'Відхилення', 'y', 'W(y)');
fprintf('-------------------------------------------\n');
for i = 1:length(y_pts)
    fprintf('%10.1fs | %10.4f | %10.4f\n', k_y(i), y_pts(i), Wy(i));
end

% 2.11
sig_y_res = sig_y * sqrt(1 - r^2); 

k_vec = [-3, -2, -1, -0.8, -0.6, -0.4, -0.2, 0, 0.2, 0.4, 0.6, 0.8, 1, 2, 3];

y_K1 = My_rish_K1 + k_vec * sig_y;
W_y_K1 = normpdf(y_K1, My_rish_K1, sig_y_res);

y_K2 = My_rish_K2 + k_vec * sig_y;
W_y_K2 = normpdf(y_K2, My_rish_K2, sig_y_res);

fprintf('\n--- 2.11: Умовні густини W(y/ріш K1) та W(y/ріш K2) ---\n');
fprintf('%5s | %10s | %10s | %10s | %10s\n', 'k', 'y(K1)', 'W(y/K1)', 'y(K2)', 'W(y/K2)');
fprintf('-------------------------------------------------------------\n');
for i = 1:length(k_vec)
    fprintf('%5.1f | %10.4f | %10.4f | %10.4f | %10.4f\n', ...
        k_vec(i), y_K1(i), W_y_K1(i), y_K2(i), W_y_K2(i));
end

% 2.12

X_test = [5.0, 4.0, 3.0, 2.0];

W_x_K1_test = normpdf(X_test, Mx_K1, sig_x_K1);
W_x_K2_test = normpdf(X_test, Mx_K2, sig_x_K2);

lambda_x = W_x_K1_test ./ W_x_K2_test;

fprintf('\n--- 2.12: Відношення правдоподібності lambda(x) ---\n');
fprintf('%10s | %10s | %10s | %15s\n', 'x', 'W(x/K1)', 'W(x/K2)', 'lambda(x)');
fprintf('------------------------------------------------------------\n');
for i = 1:length(X_test)
    fprintf('%10.1f | %10.4f | %10.4f | %15.4e\n', ...
        X_test(i), W_x_K1_test(i), W_x_K2_test(i), lambda_x(i));
end

% 2.13

lambda_por = P_K2 / P_K1;

decision = strings(1, length(X_test));

for i = 1:length(X_test)
    if lambda_x(i) >= lambda_por
        decision(i) = "K1 (Придатна)";
    else
        decision(i) = "K2 (Дефектна)";
    end
end

fprintf('\n--- 2.13 ---\n');
fprintf('Порогове значення lambda_por = %.4f\n', lambda_por);
fprintf('------------------------------------------------------------\n');
fprintf('%10s | %15s | %15s | %15s\n', 'x', 'lambda(x)', 'lambda_por', 'Рішення');
fprintf('------------------------------------------------------------\n');
for i = 1:length(X_test)
    fprintf('%10.1f | %15.4e | %15.4f | %15s\n', ...
        X_test(i), lambda_x(i), lambda_por, decision(i));
end

% 2.14

decision_X = strings(1, length(X_test));
for i = 1:length(X_test)
    if X_test(i) <= X_kl 
        decision_X(i) = "K1 (Придатна)";
    else
        decision_X(i) = "K2 (Дефектна)";
    end
end

fprintf('\n--- 2.14: Порівняння результатів класифікації ---\n');
fprintf('%5s | %10s | %12s | %12s | %15s\n', 'x', 'lambda(x)', 'Поріг X_kl', 'Рішення п.14', 'Рішення п.13');
fprintf('--------------------------------------------------------------------------\n');
for i = 1:length(X_test)
    fprintf('%5.1f | %10.4e | %12.4f | %12s | %15s\n', ...
        X_test(i), lambda_x(i), X_kl, decision_X(i), decision(i));
end

if all(decision_X == decision)
    fprintf('\nРезультати класифікації за X_kl та за lambda(x) збігаються.\n');
else
    fprintf('\nЄ розбіжності в результатах класифікації!\n');
end

% 2.15
r_values = [r, 0.8*r, 0.6*r, 0.4*r, 0.2*r, 0.1*r];
diff_M = zeros(1, length(r_values));

z_val = (Y_edge - My) / sig_y;
W_z_val = normpdf(z_val);
P_K1_val = normcdf(z_val);
P_K2_val = 1 - P_K1_val;

for i = 1:length(r_values)
    curr_r = r_values(i);
    My_K1 = My - curr_r * sig_y * (W_z_val / P_K1_val);
    My_K2 = My + curr_r * sig_y * (W_z_val / P_K2_val);
    diff_M(i) = abs(My_K1 - My_K2);
end

fprintf('\n--- 2.15: Вплив r на модуль різниці умовних сподівань ---\n');
fprintf('%10s | %15s\n', 'r', '|My/K1 - My/K2|');
fprintf('--------------------------------------\n');
for i = 1:length(r_values)
    fprintf('%10.2f | %15.4f\n', r_values(i), diff_M(i));
end

figure(1);
plot(abs(r_values), diff_M, '-ok', 'LineWidth', 2);
grid on;
xlabel('|r|');
ylabel('|M[y/K1] - M[y/K2]|');
title("Залежність роздільної здатності від тісноти зв'язку");


% 2.16

figure(2);
hold on; grid on;
x_plot = linspace(Mx - 4*sig_x, Mx + 4*sig_x, 500);

plot(x_plot, normpdf(x_plot, Mx, sig_x), 'k--', 'LineWidth', 2); % W(x)
plot(x_plot, normpdf(x_plot, Mx_K1, sig_x_K1), 'b', 'LineWidth', 2); % W(x/K1)
plot(x_plot, normpdf(x_plot, Mx_K2, sig_x_K2), 'r', 'LineWidth', 2); % W(x/K2)
xline(X_kl, 'g-', ['Xкл = ', num2str(X_kl, 4)], 'LineWidth', 2);

title('Розподіли ознаки X (Шум)');
xlabel('X'); ylabel('W(x)');
legend('W(x)', 'W(x/K1)', 'W(x/K2)', 'Поріг Xкл');
hold off;


figure(3);
hold on; grid on;
y_plot = linspace(My - 4*sig_y, My + 4*sig_y, 500);

plot(y_plot, normpdf(y_plot, My, sig_y), 'k--', 'LineWidth', 2); % W(y)
plot(y_plot, normpdf(y_plot, My_rish_K1, sig_y_res), 'b', 'LineWidth', 2); % W(y/ріш K1)
plot(y_plot, normpdf(y_plot, My_rish_K2, sig_y_res), 'r', 'LineWidth', 2); % W(y/ріш K2)
xline(My - sig_x, 'm--', 'Yгр', 'LineWidth', 1.5);

title('Розподіли параметра Y (Старіння)');
xlabel('Y'); ylabel('W(y)');
legend('W(y)', 'W(y/ріш K1)', 'W(y/ріш K2)', 'Межа Yгр');
hold off;