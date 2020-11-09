from django.shortcuts import render

# Create your views here.

from django.shortcuts import render

def hello(request):
    context          = {}
    context['hello'] = 'My Blog'
    return render(request, 'gaimima.html', context)

