clc; clear; close all;

%% 仿真参数
dt = 0.1;          % 采样时间
T = 10;            % 总时间
N = T / dt;        % 仿真步数

% 控制输入（常数）
v_input = 1.0;     % m/s
w_input = 0.5;     % rad/s
u = [v_input; w_input];

% 初始化状态向量 x = [x; y; yaw; v; w]
x_real = [0; 0; 0; v_input; w_input];   % 初始真实状态
x_hat  = [0; 0; 0; 0; 0];              % 初始估计状态（观测器）

% 观测矩阵（仅观测 v 和 w）
C = [0 0 0 1 0;
     0 0 0 0 1];

% 系统矩阵（简化线性模型）
A = zeros(5);  % 状态转移矩阵（线性化模型，假设 x, y, yaw 不反馈到 v, w）
B = [0 0;
     0 0;
     0 0;
     1 0;
     0 1];

% 观测器增益 L（选择使误差收敛较快）
L = place(A', C', [-2 -2.5])';  % 极点配置

% 存储数据
x_real_history = zeros(N, 5);
x_hat_history  = zeros(N, 5);
v_history      = zeros(N, 2);  % [真实v, 估计v]
w_history      = zeros(N, 2);  % [真实w, 估计w]

%% 仿真循环
for k = 1:N
    % 实际系统更新（非线性运动学）
    x = x_real;
    theta = x(3);
    dx = [x(4) * cos(theta);
          x(4) * sin(theta);
          x(5);
          0;
          0];
    x_real = x_real + dt * dx;

    % 输出观测（真实 v 和 w）
    y = C * x_real;

    % 观测器预测（基于线性模型）
    x_hat = x_hat + dt * (A * x_hat + B * u + L * (y - C * x_hat));

    % 存储历史数据
    x_real_history(k, :) = x_real';
    x_hat_history(k, :)  = x_hat';
    v_history(k, :) = [x_real(4), x_hat(4)];
    w_history(k, :) = [x_real(5), x_hat(5)];
end

%% 画图：v 和 w 对比
figure;
subplot(1, 2, 1);
plot(v_history(:, 1), 'b', 'LineWidth', 1.5); hold on;
plot(v_history(:, 2), 'r--', 'LineWidth', 1.5);
title('v 观测值 vs 实际值');
xlabel('时间步'); ylabel('v (m/s)');
legend('实际 v', '观测 v'); grid on;

subplot(1, 2, 2);
plot(w_history(:, 1), 'b', 'LineWidth', 1.5); hold on;
plot(w_history(:, 2), 'r--', 'LineWidth', 1.5);
title('w 观测值 vs 实际值');
xlabel('时间步'); ylabel('w (rad/s)');
legend('实际 w', '观测 w'); grid on;

%% 小车轨迹对比
figure;
plot(x_real_history(:,1), x_real_history(:,2), 'b', 'LineWidth', 2); hold on;
plot(x_hat_history(:,1), x_hat_history(:,2), 'r--', 'LineWidth', 2);
xlabel('x (m)'); ylabel('y (m)');
title('小车轨迹：真实 vs 估计');
legend('真实轨迹', '估计轨迹');
axis equal; grid on;
