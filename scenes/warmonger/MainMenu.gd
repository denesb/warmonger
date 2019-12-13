extends Control

onready var ctx = preload("res://interface/wcontext.gdns").new()

func _ready():
    $VersionLabel.text = ctx.version