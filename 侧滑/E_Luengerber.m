clc; clear; close all;

%% 参数
Lr = 0.5;          % 后轴距
k_beta = 1.5;      % β 的动态响应速度
dt = 0.01;         % 时间步长
T = 10;            % 模拟总时间
N = T / dt;        % 步数

%% 状态变量初始化
x_true = [0; 1.0; 0.0];   % [psi; v; beta] 真值
x_hat  = [0; 2; 5];   % 观测器初始估计

%% 观测器增益
L = [0.5  0;
     2.0  0;
     0    5.0];   % 正确：这是 3x2 矩阵
   % Luenberger 增益矩阵，对应 [v; omega] 的校正

%% 记录变量
X_true = zeros(3, N);
X_hat = zeros(3, N);
Beta_error = zeros(1, N);

%% 仿真开始
for k = 1:N
    %---------------------
    % 真值系统推进
    %---------------------
    psi = x_true(1);
    v = x_true(2);
    beta = x_true(3);
    
    omega = (v / Lr) * sin(beta); % 真正的 omega
    
    % 状态更新
    x_true(1) = x_true(1) + dt * omega;
    x_true(2) = x_true(2); % 匀速
    x_true(3) = x_true(3) + dt * k_beta * (-x_true(3)); % 假设 δ = 0

    %---------------------
    % 观测器更新
    %---------------------
    % 测量
    y = [x_true(2);
         (x_true(2) / Lr) * sin(x_true(3))];
    
    % 预测输出
    v_hat = x_hat(2);
    beta_hat = x_hat(3);
    y_hat = [v_hat;
             (v_hat / Lr) * sin(beta_hat)];
    
    % 状态预测更新
    omega_hat = (v_hat / Lr) * sin(beta_hat);
    dot_hat = [
        omega_hat;
        0;
        k_beta * (-beta_hat)   % δ = 0
    ];
    
    x_hat = x_hat + dt * (dot_hat + L * (y - y_hat));
    
    %---------------------
    % 记录
    %---------------------
    X_true(:,k) = x_true;
    X_hat(:,k) = x_hat;
    Beta_error(k) = x_hat(3) - x_true(3);
end

%% 可视化
time = (0:N-1) * dt;

figure;
plot(time, X_true(3,:), 'b-', 'LineWidth', 2); hold on;
plot(time, X_hat(3,:), 'r--', 'LineWidth', 2);
legend('真实β', '估计β');
xlabel('时间 (s)'); ylabel('侧滑角 β (rad)');
title('侧滑角估计 vs 真值');
grid on;

figure;
plot(time, Beta_error, 'k', 'LineWidth', 1.5);
xlabel('时间 (s)'); ylabel('估计误差');
title('β 估计误差');
grid on;
