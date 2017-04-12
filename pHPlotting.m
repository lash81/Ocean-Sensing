%% pH Plotting

time = [15	30	45	60	75	90	105	120	135	150	165];

ph3_6a = [0.58	0.595	0.601	0.605	0.608	0.609	0.611	0.612	0.613	0.613	0.615];
ph3_6b = [0.561	0.578	0.59	0.597	0.604	0.609	0.611	0.613	0.615	0.618	0.618];
ph3_6c = [0.568	0.575	0.584	0.591	0.597	0.602	0.606	0.61	0.613	0.614	0.617];

ph4_4a = [0.539	0.554	0.564	0.571	0.577	0.58	0.585	0.588	0.59	0.591	0.595];
ph4_4b = [0.562	0.576	0.584	0.59	0.593	0.597	0.599	0.601	0.602	0.604	0.605];
ph4_4c = [0.575	0.585	0.591	0.596	0.599	0.6	0.603	0.604	0.604	0.605	0.606];

ph5_9a = [0.502	0.507	0.515	0.519	0.523	0.523	0.527	0.529	0.53	0.531	0.532];


figure(1)
plot(time,ph3_6a,'r:')
hold on
plot(time,ph3_6b,'r-.')
plot(time,ph3_6c,'r-')
plot(time,ph4_4a,'b:')
plot(time,ph4_4b,'b-.')
plot(time,ph4_4c,'b-')
plot(time,ph5_9a,'g-')
hold off
legend('pH 3.6 trial 1', 'pH 3.6 trial 2', 'pH 3.6 trial 3', 'pH 4.4 trial 1','pH 4.4 trial 2','pH 4.4 trial 3','pH 5.9',0)
title('Voltage between Ti and Zn rods over time in different pH solutions')
ylabel('Voltage (V)')
xlabel('Time (s)')

