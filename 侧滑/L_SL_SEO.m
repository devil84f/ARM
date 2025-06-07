clc; clear; close all;

%% 系统参数
% 系统矩阵
A = [0 1; 0 -2];
B = [0; 1];
C = [1 0];
D = 0;

% 仿真时间设置
t = 0:0.01:20;       % 时间向量，步长0.01s
dt = t(2) - t(1);    % 计算步长
u = ones(size(t));   % 阶跃输入信号
d = 0.5 + sin(2*t);  % 0.5 + sin(2*t);    % 外部扰动

% 初始化状态和输出存储
x = zeros(2, length(t));  % 状态变量 [x1; x2]
y = zeros(1, length(t));  % 输出 y = x1
x(:,1) = [0; 0.5];          % 初始状态 x1(0)=0, x2(0)=5

for k = 1:length(t)-1
    % 计算状态导数（含扰动d(t)）
    dx = A * x(:,k) + B * (u(k) + 0);
    
    % 欧拉法更新状态
    x(:,k+1) = x(:,k) + dx * dt;
    
    % 计算输出
    y(k) = C * x(:,k) + D * u(k);
end
y(end) = C * x(:,end) + D * u(end); % 补全最后一个输出

%% 龙伯格观测器增益
L = [8; 9];  % 观测器增益（极点配置为-5）
x_hat_luenberger = zeros(2, length(t));
x_hat_luenberger(:,1) = [0; 0]; % 观测器初始状态

for k = 1:length(t)-1
    e = y(k) - C * x_hat_luenberger(:,k);
    dx_hat = A * x_hat_luenberger(:,k) + B * u(k) + L * e;
    x_hat_luenberger(:,k+1) = x_hat_luenberger(:,k) + dx_hat * dt;
end

%% 非线性观测器参数
beta1 = 150; beta2 = 150;
x_hat_nonlinear = zeros(2, length(t));
x_hat_nonlinear(:,1) = [0; 0];

for k = 1:length(t)-1
    e = y(k) - C * x_hat_nonlinear(:,k);
    dx_hat = [x_hat_nonlinear(2,k) + beta1*e;
              u(k) + beta2*sign(e)*((abs(e))^(1/2))];
    x_hat_nonlinear(:,k+1) = x_hat_nonlinear(:,k) + dx_hat * dt;
end

%% ESO 参数
beta1 = 50; beta2 = 50; beta3 = 50;
x_hat_eso = zeros(3, length(t));
x_hat_eso(:,1) = [0; 0; 0];

for k = 1:length(t)-1
    e = x_hat_eso(1,k) - y(k);  % 输出误差
    dx_hat = [x_hat_eso(2,k) - beta1 * e;                     % x1 动态
              x_hat_eso(3,k) + u(k) - beta2 * abs(e)^(1/2)*sign(e);  % x2 动态
              -beta3 * abs(e)^(1/4)*sign(e)];                     % x3（扰动估计）
    x_hat_eso(:,k+1) = x_hat_eso(:,k) + dx_hat * (t(k+1)-t(k));  % 欧拉积分
end

%% 绘图
% 状态x2的观测对比
figure;
plot(t, x(2,:), 'k', 'LineWidth', 2); hold on;
plot(t, x_hat_luenberger(2,:), 'r--');
plot(t, x_hat_nonlinear(2,:), 'b--');
plot(t, x_hat_eso(2,:), 'g--');
legend('真实x2', '龙伯格观测', '非线性观测', 'ESO观测');
title('状态x2的观测效果');
xlabel('时间 (s)'); grid on;

% ESO扰动估计
figure;
plot(t, d, 'k', 'LineWidth', 2); hold on;
plot(t, x_hat_eso(3,:), 'g--');
legend('真实扰动', 'ESO估计扰动');
title('ESO扰动估计效果');
xlabel('时间 (s)'); grid on;

%% 定义 fal 函数（ESO 非线性项）
function f = fal(e, alpha, delta)
    if abs(e) > delta
        f = abs(e)^alpha * sign(e);
    else
        f = e / (delta^(1-alpha));
    end
end