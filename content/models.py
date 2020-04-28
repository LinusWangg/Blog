from django.db import models
from ckeditor.fields import RichTextField
from mdeditor.fields import MDTextField

# Create your models here.

class Article(models.Model):
    objects = models.Manager()
    title = models.CharField(max_length=64)
    content =  MDTextField()
    createtime = models.DateTimeField(auto_now=True)
    modifytime = models.DateTimeField(auto_now=True)
    commitnum = models.IntegerField(default=0)
    url = models.URLField(default="http://127.0.0.1:8000/index/info")

class kuilei(models.Model):
    objects = models.Manager()
    title = models.CharField(max_length=64)
    url = models.URLField(default="http://127.0.0.1:8000/index/info")