﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TurboBudget.Types
{
    public interface INamed
    {
        string Name { get; set; }
        string Description { get; set; }
    }
}
