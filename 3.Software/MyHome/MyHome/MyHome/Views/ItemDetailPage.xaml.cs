using MyHome.ViewModels;
using System.ComponentModel;
using Xamarin.Forms;

namespace MyHome.Views
{
    public partial class ItemDetailPage : ContentPage
    {
        public ItemDetailPage()
        {
            InitializeComponent();
            BindingContext = new ItemDetailViewModel();
        }
    }
}