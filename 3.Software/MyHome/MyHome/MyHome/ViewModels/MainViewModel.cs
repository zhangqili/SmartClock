using AlibabaCloud.SDK.Iot20180120;
using AlibabaCloud.SDK.Iot20180120.Models;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using Xamarin.Forms;

namespace MyHome.ViewModels
{
    internal class MainViewModel : BaseViewModel
    {
        public Command LEDCommand { get; }
        public Command AlarmCommand { get; }
        public Command RefreshCommand { get; }

        private bool isRefreshing;

        public bool IsRefreshing
        {
            get => isRefreshing;
            set => SetProperty(ref isRefreshing, value);
        }

        private string deviceStatus;

        public string DeviceStatus
        {
            get => deviceStatus;
            set => SetProperty(ref deviceStatus, value);
        }


        private string humidity;

        public string Humidity
        {
            get => humidity;
            set => SetProperty(ref humidity, value);
        }

        private string temperature;
        public string Temperature
        {
            get => temperature;
            set => SetProperty(ref temperature, value);
        }

        private TimeSpan timeSpan;
        public TimeSpan TimeSpan
        {
            get => timeSpan;
            set
            {
                SetProperty(ref timeSpan, value);
            }
        }

        private string timeText;

        public string TimeText
        {
            get => timeText;
            set
            {
                SetProperty(ref timeText, value);
            }
        }

        private string lED;

        public string LED
        {
            get => lED;
            set => SetProperty(ref lED, value);
        }

        public static AlibabaCloud.SDK.Iot20180120.Client CreateClient(string accessKeyId, string accessKeySecret)
        {
            AlibabaCloud.OpenApiClient.Models.Config config = new AlibabaCloud.OpenApiClient.Models.Config
            {
                // 您的AccessKey ID
                AccessKeyId = accessKeyId,
                // 您的AccessKey Secret
                AccessKeySecret = accessKeySecret,
            };
            // 访问的域名
            config.Endpoint = "iot.cn-shanghai.aliyuncs.com";
            return new AlibabaCloud.SDK.Iot20180120.Client(config);
        }
        public MainViewModel()
        {
            LEDCommand = new Command(OnChange);
            AlarmCommand = new Command(UpdateAlarm);
            RefreshCommand = new Command(()=>Tick_tick(null));
            Timer timer = new Timer(Tick_tick, null, 0, 10000);
            
            LED = "OFF";
            TimeSpan = TimeSpan.FromMinutes((DateTime.Now-DateTime.Today).Minutes);
            try
            {
                timer.InitializeLifetimeService();
            }
            catch (PlatformNotSupportedException)
            {
                ;
            }
        }

        private void Tick_tick(object state)
        {
            AlibabaCloud.SDK.Iot20180120.Client client = CreateClient("LTAI5tDxgXQhAPK89WZx9j9V", "wF6FlNXrEV563oZEraYGMJhbqAdYKu");
            AlibabaCloud.SDK.Iot20180120.Models.QueryDevicePropertyStatusRequest queryDevicePropertyStatusRequest = new AlibabaCloud.SDK.Iot20180120.Models.QueryDevicePropertyStatusRequest
            {
                IotInstanceId = "iot-06z00fac0zmj2le",
                ProductKey = "h9xwJufiIbz",
                DeviceName = "ESP8266",
            };
            AlibabaCloud.TeaUtil.Models.RuntimeOptions runtime = new AlibabaCloud.TeaUtil.Models.RuntimeOptions();
            // 复制代码运行请自行打印 API 的返回值
            QueryDevicePropertyStatusResponse query = client.QueryDevicePropertyStatusWithOptions(queryDevicePropertyStatusRequest, runtime);
            foreach (var item in query.Body.Data.List.PropertyStatusInfo)
            {
                switch (item.Identifier)
                {
                    case "CurrentHumidity":
                        Humidity = DoubleToString(Convert.ToDouble(item.Value)) + item.Unit;
                        break;
                    case "CurrentTemperature":
                        Temperature = DoubleToString(Convert.ToDouble(item.Value)) + item.Unit;
                        break;
                    case "LEDSwitch":
                        //LED = item.Value == "0"?"ON":"OFF";
                        break;
                    default:
                        break;
                }
            }
            AlibabaCloud.SDK.Iot20180120.Models.GetDeviceStatusRequest getDeviceStatusRequest = new AlibabaCloud.SDK.Iot20180120.Models.GetDeviceStatusRequest()
            {
                IotInstanceId = "iot-06z00fac0zmj2le",
                ProductKey = "h9xwJufiIbz",
                DeviceName = "ESP8266",
            };
            GetDeviceStatusResponse statusResponse = client.GetDeviceStatusWithOptions(getDeviceStatusRequest,runtime); 
            
            DateTimeOffset dateTimeOffset = DateTimeOffset.FromUnixTimeMilliseconds(Convert.ToInt64(statusResponse.Body.Data.Timestamp));
            DateTime dateTime = dateTimeOffset.DateTime.AddHours(8);

            DeviceStatus = "该设备状态为" + statusResponse.Body.Data.Status + " 自" + dateTime.ToLongDateString() + " " + dateTime.ToLongTimeString();
            IsRefreshing = false;
        }

        private string DoubleToString(double x)
        {
            return String.Format("{0:F1}",x);
        }

        private void OnChange()
        {
            LED = (LED == "OFF") ? "ON" : "OFF";
            AlibabaCloud.SDK.Iot20180120.Client client = CreateClient("LTAI5tDxgXQhAPK89WZx9j9V", "wF6FlNXrEV563oZEraYGMJhbqAdYKu");
            AlibabaCloud.SDK.Iot20180120.Models.SetDevicePropertyRequest setDevicePropertyRequest = new SetDevicePropertyRequest
            {
                IotInstanceId = "iot-06z00fac0zmj2le",
                ProductKey = "h9xwJufiIbz",
                DeviceName = "ESP8266",
                Items = CreateLEDjson(LED)
            };
            
            AlibabaCloud.TeaUtil.Models.RuntimeOptions runtime = new AlibabaCloud.TeaUtil.Models.RuntimeOptions();
            // 复制代码运行请自行打印 API 的返回值
            var query = client.SetDevicePropertyWithOptions(setDevicePropertyRequest, runtime);
        }

        private string CreateLEDjson(string flag)
        {
            switch (flag)
            {
                case "ON":
                    return "{" + "\"LEDSwitch\":" + "0" + "}";
                case "OFF":
                    return "{" + "\"LEDSwitch\":" + "1" + "}";
            }
            return "{" + "\"LEDSwitch\":" + "0" + "}";
        }

        private void UpdateAlarm()
        {
            TimeText = TimeSpan.TotalMinutes.ToString();
            
            AlibabaCloud.SDK.Iot20180120.Client client = CreateClient("LTAI5tDxgXQhAPK89WZx9j9V", "wF6FlNXrEV563oZEraYGMJhbqAdYKu");
            AlibabaCloud.SDK.Iot20180120.Models.SetDevicePropertyRequest setDevicePropertyRequest = new SetDevicePropertyRequest
            {
                IotInstanceId = "iot-06z00fac0zmj2le",
                ProductKey = "h9xwJufiIbz",
                DeviceName = "ESP8266",
                Items = CreateAlarmjson(TimeText)
            };

            AlibabaCloud.TeaUtil.Models.RuntimeOptions runtime = new AlibabaCloud.TeaUtil.Models.RuntimeOptions();
            // 复制代码运行请自行打印 API 的返回值
            var query = client.SetDevicePropertyWithOptions(setDevicePropertyRequest, runtime);
            
        }
        private string CreateAlarmjson(string flag)
        {
            return "{" + "\"AlarmTimeMinutes\":" + flag + "}";
        }
    }
}
