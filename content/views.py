from django.shortcuts import render
from .import models
from .models import Article,kuilei
import markdown

# Create your views here.

def content(request):
    content_list=Article.objects.all()
    return render(request,"content.html",{"content_list":content_list})

def detail(request,article_id):
    article = models.Article.objects.get(pk=article_id)
    article.content = markdown.markdown(article.content,
                                extensions=[
                                    'markdown.extensions.extra',
                                    'markdown.extensions.codehilite',
                                    'markdown.extensions.toc',
                                ])
    temp = int(article_id)
    p = (str)(temp-1)
    a = (str)(temp+1)
    pre = models.Article.objects.filter(pk=p).first()
    after = models.Article.objects.filter(pk=a).first()
    if not pre:
        print(1)
        pre = kuilei(title="空")
    else:
        if pre.url=="http://127.0.0.1:8000/index/info":
            models.Article.objects.filter(pk=p).update(url="http://127.0.0.1:8000/content/content/"+p)
            pre = models.Article.objects.filter(pk=p).first()
    if not after:
        print(2)
        after = kuilei(title="空")
    else:
        if after.url=="http://127.0.0.1:8000/index/info":
            models.Article.objects.filter(pk=a).update(url="http://127.0.0.1:8000/content/content/"+a)
            after = models.Article.objects.filter(pk=a).first()
    return render(request, 'detail.html', {'article': article,'pre':pre,'after':after})